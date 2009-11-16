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

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "serial.h"
#include "protocol.h"

int main(int argc, char **argv){
    u16 val;
    int i;

    if(serial_open()){
        return 1;
    }

    serial_clear();

    for(i=0; i<100; i++){
        if(gp_set(1, i) < 0){
            fprintf(stderr, "gp_set failed so closing.\n");
            serial_close();
            return EXIT_FAILURE;
        }
        if(gp_get(1, &val) < 0){
            fprintf(stderr, "gp_get failed so closing.\n");
            serial_close();
            return EXIT_FAILURE;
        }

        printf("Got value %d(0x%04X) for register 1\n", val, val);
        /*
        if(gp_get(1, &val) < 0){
            fprintf(stderr, "gp_get failed so closing.\n");
            serial_close();
            return EXIT_FAILURE;
        }
        printf("Got value %d(0x%04X) for register 1\n", val, val);
        */
    }

    serial_close();

    return 0;
}
