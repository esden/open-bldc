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

/*
 * 0000 0000
 * ^^^^ ^^^^
 * |||'-''''- Register
 * ||'------- Reserved
 * |'-------- 0 ^= Peek 1 ^= Continous
 * '--------- 0 ^= Read 1 ^= Write
 */

#include <stdio.h>
#include <stdlib.h>

#include "serial.h"

int main(int argc, char **argv){
    int ret;
    unsigned char *buff;
    int i, j;

    buff = malloc(11);

    if(serial_open()){
        return 1;
    }

    for(i=0; i<100; i++){
        if(serial_write((unsigned char *)"1234567890", 10) < 0){
            serial_close();
            return EXIT_FAILURE;
        }
        ret = serial_read(buff, 10);
        if(ret < 0){
            serial_close();
            return EXIT_FAILURE;
        }
        if(ret > 0){
            printf("got %04d : ", i);
            for(j=0; j < ret; j++){
                putchar((char)buff[j]);
            }
            printf("\n");
        }
    }

    serial_close();

    return 0;
}
