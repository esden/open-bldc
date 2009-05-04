/*
 * Open-BLDC Tool - Open BruschLess DC Motor Controller PC Tool
 * Copyright (C) 2009 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "serial.h"

#ifdef G_OS_WIN32
#include <windows.h>
#include "ftd2xx_win.h"
#else
#include "ftd2xx.h"
#endif

#define FT_READ_TIMEOUT 2000
#define FT_WRITE_TIMEOUT 2000

unsigned long ft_current_timeout;

FT_HANDLE ft_handle = NULL;

char *ft_return[] = {
    "FT_OK",
    "FT_INVALID_HANDLE",
    "FT_DEVICE_NOT_FOUND",
    "FT_DEVICE_NOT_OPENED",
    "FT_IO_ERROR",
    "FT_INSUFFICIENT_RESOURCES",
    "FT_INVALID_PARAMETER",
    "FT_INVALID_BAUD_RATE",
    "FT_DEVICE_NOT_OPENED_FOR_ERASE",
    "FT_DEVICE_NOT_OPENED_FOR_WRITE",
    "FT_FAILED_TO_WRITE_DEVICE",
    "FT_EEPROM_READ_FAILED",
    "FT_EEPROM_WRITE_FAILED",
    "FT_EEPROM_ERASE_FAILED",
    "FT_EEPROM_NOT_PRESENT",
    "FT_EEPROM_NOT_PROGRAMMED",
    "FT_INVALID_ARGS",
    "FT_NOT_SUPPORTED",
    "FT_OTHER_ERROR"
};

int s_get_list(char **dev_list){
    int num_devs = 0;
    FT_STATUS ft_status;

    ft_status = FT_ListDevices(dev_list, &num_devs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);

    if(ft_status != FT_OK){
        printf("Error: FT_ListDevices(%s)\n", ft_return[ft_status]);
        return 0;
    }

    return num_devs;
}

S_STATUS s_open(char *dev_name, double baud){
    FT_STATUS ft_status;

    if(ft_handle != NULL){
        printf("Error device already open\n");
        return S_OPEN;
    }

    if(dev_name == NULL){
        printf("Device name invalid\n");
        return S_INVALID_PARAMETER;
    }

    ft_status = FT_OpenEx(dev_name, FT_OPEN_BY_SERIAL_NUMBER, &ft_handle);

    if(ft_status != FT_OK){
        printf("Error FT_OpenEx(%s), device %s\n", ft_return[ft_status], dev_name);
        return ft_status;
    }

    ft_status = FT_SetDataCharacteristics(ft_handle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);

    if(ft_status != FT_OK){
        printf("Error FT_SetDataCharacteristics(%s), device%s\n", ft_return[ft_status], dev_name);
        FT_Close(ft_handle);
        ft_handle = NULL;
        return ft_status;
    }

    ft_status = FT_SetBaudRate(ft_handle, baud);

    if(ft_status != FT_OK){
        printf("Error FT_SetBaudRate(%s), device %s\n", ft_return[ft_status], dev_name);
        FT_Close(ft_handle);
        ft_handle = NULL;
        return ft_status;
    }

    ft_current_timeout = FT_READ_TIMEOUT;
    ft_status = FT_SetTimeouts(ft_handle, FT_READ_TIMEOUT, FT_WRITE_TIMEOUT);

    if(ft_status != FT_OK){
        printf("Error FT_SetTimeouts(%s), device %s\n", ft_return[ft_status], dev_name);
        FT_Close(ft_handle);
        ft_handle = NULL;
        return ft_status;
    }

    return S_OK;
}

S_STATUS s_set_timeout(unsigned long timeout){
    FT_STATUS ft_status;

    if(ft_handle == NULL){
        printf("Error no device open\n");
        return S_CLOSED;
    }

    ft_current_timeout = timeout;
    if(timeout < 1000){
        ft_status = FT_SetTimeouts(ft_handle, 0, FT_WRITE_TIMEOUT);
    }else{
        ft_status = FT_SetTimeouts(ft_handle, timeout, FT_WRITE_TIMEOUT);
    }

    if(ft_status != FT_OK){
        printf("Error FT_SetTimeouts(%s)\n", ft_return[ft_status]);
        FT_Close(ft_handle);
        ft_handle = NULL;
        return ft_status;
    }

    return S_OK;
}

S_STATUS s_reset_timeout(void){
    FT_STATUS ft_status;

    if(ft_handle == NULL){
        printf("Error no device open\n");
        return S_CLOSED;
    }

    ft_current_timeout = FT_READ_TIMEOUT;
    ft_status = FT_SetTimeouts(ft_handle, FT_READ_TIMEOUT, FT_WRITE_TIMEOUT);

    if(ft_status != FT_OK){
        printf("Error FT_SetTimeouts(%s)\n", ft_return[ft_status]);
        FT_Close(ft_handle);
        ft_handle = NULL;
        return ft_status;
    }

    return S_OK;
}

S_STATUS s_close(void){
    if(ft_handle == NULL){
        printf("Error no device open.\n");
        return S_CLOSED;
    }

    FT_Close(ft_handle);
    ft_handle = NULL;

    return S_OK;
}

s_size_t s_write(unsigned char *data, s_size_t size){
    FT_STATUS ft_status;
    s_size_t written;

    if(ft_handle == NULL){
        printf("Error no device open\n");
        return 0;
    }

    ft_status = FT_Write(ft_handle, data, size, &written);

    if(ft_status != FT_OK){
        printf("Error FT_Write(%s)\n", ft_return[ft_status]);
        return 0;
    }

    return written;
}

s_size_t s_read(unsigned char *data, s_size_t size){
    FT_STATUS ft_status = FT_OK;
    s_size_t read = 0;
    s_size_t tmp_read;
    unsigned long timeout = 0;
    s_size_t queue_fill;

    if(ft_handle == NULL){
        printf("Error no device open\n");
        return 0;
    }

    if(ft_current_timeout < 1000){
        while((read < size) && (timeout < ft_current_timeout)){
            ft_status = FT_GetQueueStatus(ft_handle, &queue_fill);
            if(ft_status != FT_OK){
                printf("Error FT_GetQueueStatus(%s)\n", ft_return[ft_status]);
                return 0;
            }

            if(queue_fill > 0){
                timeout = 0;
                if((read + queue_fill) > size){
                    ft_status = FT_Read(ft_handle, data+read, size - read, &tmp_read);
                }else{
                    ft_status = FT_Read(ft_handle, data+read, queue_fill, &tmp_read);
                }
                if(ft_status != FT_OK){
                    printf("Error FT_Read(%s)\n", ft_return[ft_status]);
                    return 0;
                }
                read += tmp_read;
            }else{
                usleep(10000);
                timeout += 10;
            }
        }
    }else{
        ft_status = FT_Read(ft_handle, data, size, &read);
        if(ft_status != FT_OK){
            printf("Error FT_Read(%s)\n", ft_return[ft_status]);
            return 0;
        }
    }

    return read;
}

S_STATUS s_purge(void){
    FT_STATUS ft_status;

    if(ft_handle == NULL){
        printf("Error no device open\n");
        return S_CLOSED;
    }

    ft_status = FT_Purge(ft_handle, FT_PURGE_RX | FT_PURGE_TX);

    if(ft_status != FT_OK){
        printf("Error FT_Purge(%s)\n", ft_return[ft_status]);
	return ft_status;
    }

    return S_OK;
}

s_size_t s_queue_status(void){
    FT_STATUS ft_status;
    s_size_t queue_size;

    if(ft_handle == NULL){
        printf("Error no device open\n");
        return 0;
    }

    ft_status = FT_GetQueueStatus(ft_handle, &queue_size);

    if(ft_status != FT_OK){
        printf("Error FT_GetQueueStatus(%s)\n", ft_return[ft_status]);
        return 0;
    }

    return queue_size;
}
