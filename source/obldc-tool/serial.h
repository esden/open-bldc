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

#ifndef __SERIAL_H
#define __SERIAL_H

typedef unsigned long s_size_t;
typedef unsigned long S_STATUS;

enum {
    S_OK,
    S_INVALID_HANDLE,
    S_DEVICE_NOT_FOUND,
    S_DEVICE_NOT_OPENED,
    S_IO_ERROR,
    S_INSUFFICIENT_RESOURCES,
    S_INVALID_PARAMETER,
    S_INVALID_BAUD_RATE,   //7

    S_DEVICE_NOT_OPENED_FOR_ERASE,
    S_DEVICE_NOT_OPENED_FOR_WRITE,
    S_FAILED_TO_WRITE_DEVICE,
    S_EEPROM_READ_FAILED,
    S_EEPROM_WRITE_FAILED,
    S_EEPROM_ERASE_FAILED,
    S_EEPROM_NOT_PRESENT,
    S_EEPROM_NOT_PROGRAMMED,
    S_INVALID_ARGS,
    S_NOT_SUPPORTED,
    S_OTHER_ERROR,
    S_OPEN,
    S_CLOSED
};

int s_get_list(char **dev_list);
S_STATUS s_open(char *dev, double baud);
S_STATUS s_close(void);
s_size_t s_write(unsigned char *data, s_size_t size);
s_size_t s_read(unsigned char *data, s_size_t size);
S_STATUS s_purge(void);
s_size_t s_queue_status(void);
S_STATUS s_set_timeout(unsigned long timeout);
S_STATUS s_reset_timeout(void);

#endif /* __SERIAL_H */
