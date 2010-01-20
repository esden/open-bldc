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

#if (SERIAL == SERIAL_SIM)

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftdi.h>

#include "types.h"
#include "serial.h"
#include "hexdump_tool.h"

char *in_pipe_name;
char *out_pipe_name;

FILE *in_f;
FILE *out_f;

int serial_open(){
    in_pipe_name = mktemp("gin.XXXX");
    out_pipe_name = mktemp("gout.XXXX");

    mkfifo(in_pipe_name, 666);
    mkfifo(out_pipe_name, 666);

    in_f = fopen(in_pipe_name, "a+");
    if(in_f == NULL){
        perror("ERROR: opening in pipe");
    }

    out_f = fopen(out_pipe_name, "w+");

    if(out_f == NULL){
        perror("ERROR: opening out pipe");
    }

    printf("everything opened\n");
    return EXIT_SUCCESS;
}

int serial_close(){
    if(!fclose(in_f)){
        perror("ERROR: closing in pipe");
    }

    if(!fclose(out_f)){
        perror("ERROR: closing out pipe");
    }

    return EXIT_SUCCESS;
}

int serial_write(u8 *buff, size_t size){
    hexdump(buff, size);
    fwrite(buff, size, 1, out_f);

    return EXIT_SUCCESS;
}

int serial_read(u8 *buff, size_t size){
    fread(buff, size, 1, in_f);
    return EXIT_SUCCESS;
}

int serial_clear(){
    return EXIT_SUCCESS;
}

#endif /* SERIAL == SERIAL_SIM */
