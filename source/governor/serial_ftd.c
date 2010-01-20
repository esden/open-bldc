/*
 * governor - Open-BLDC PC interface tool
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

#include "config.h"

#if SERIAL == SERIAL_FTD

#include <string.h>
#include <stdio.h>
#include <ftdi.h>

#include "types.h"
#include "hexdump_tool.h"
#include "serial.h"

struct ftdi_context serial_ftdic;
int serial_ret;

int serial_open(){
    int ret;

    ftdi_init(&serial_ftdic);

    if((ret = ftdi_usb_open(&serial_ftdic, 0x0403, 0x6001)) < 0){
        fprintf(stderr, "Unable to open ftdi device: %d (%s)\n",
                ret, ftdi_get_error_string(&serial_ftdic));
        return EXIT_FAILURE;
    }

    if(serial_ftdic.type == TYPE_R){
        unsigned int chipid;
        printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(&serial_ftdic, &chipid));
        printf("FTDI chipid: %X\n", chipid);
    }

    if((ret = ftdi_set_latency_timer(&serial_ftdic, 1)) < 0){
        fprintf(stderr, "Unable to set latency timer: %d (%s)\n",
                ret, ftdi_get_error_string(&serial_ftdic));
        return EXIT_FAILURE;
    }

    if((ret = ftdi_set_baudrate(&serial_ftdic, 500000)) < 0){
        fprintf(stderr, "Unable to set baud rate: %d (%s)\n",
                ret, ftdi_get_error_string(&serial_ftdic));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int serial_close(){
    int ret;

    if((ret = ftdi_usb_close(&serial_ftdic)) < 0){
        fprintf(stderr, "Unable to close ftdi device: %d (%s)\n",
                ret, ftdi_get_error_string(&serial_ftdic));
        return EXIT_FAILURE;
    }

    ftdi_deinit(&serial_ftdic);

    return EXIT_SUCCESS;
}

int serial_write(u8 *buff, size_t size){
    int ret;

    if((ret = ftdi_write_data(&serial_ftdic, buff, size)) < 0){
        fprintf(stderr, "Failed to write data: %d (%s)\n",
                ret, ftdi_get_error_string(&serial_ftdic));
        return EXIT_FAILURE;
    }

    if(ret < size){
        fprintf(stderr, "Failed to write all data, instead wrote %d bytes.\n",
                ret);
        return EXIT_FAILURE;
    }

    return ret;
}

int serial_read(u8 *buff, size_t size){
    int ret;

    memset(buff, 0, size);
    ret = ftdi_read_data(&serial_ftdic, buff, size);
    if(ret < 0){
        fprintf(stderr, "Failed to read data: %d (%s)\n",
                ret, ftdi_get_error_string(&serial_ftdic));
    }
    hexdump(buff, ret);
    if(ret == 0){
        fprintf(stderr, "No data to read.\n");
    }
    return ret;
}

int serial_clear(void){
    u8 scrap[128];

    while(serial_read(scrap, 128) == 128){
        hexdump(scrap, 128);
    }

    return EXIT_SUCCESS;
}

#endif /* SERIAL == SERIAL_FTD */
