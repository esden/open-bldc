/*
 * Open-BLDC Tool - Open BrushLess DC Motor Controller PC Tool
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"

#define HEXDUMP_WIDTH 16

#ifndef G_OS_WIN32
#define T0 "\033[0m"
#define TG "\033[32m"
#else
#define T0
#define TG
#endif

void phexdump_line(unsigned long address, unsigned char *data, int count);
void phexdump_printchar(unsigned char ch);
void phexdump_printhex(unsigned char ch);

void hexdump(unsigned char *data, unsigned long count){
    unsigned long i;

    printf("\n");
    for(i = 0; i < ((count / HEXDUMP_WIDTH) + 1); i++){
        if(((i + 1) * HEXDUMP_WIDTH) <= count){
            phexdump_line(i * HEXDUMP_WIDTH, data + (i * HEXDUMP_WIDTH), 
                          HEXDUMP_WIDTH);
        }else{
            phexdump_line(i * HEXDUMP_WIDTH, data + (i * HEXDUMP_WIDTH), 
                          count % HEXDUMP_WIDTH);
        }
    }
    printf("\n");
}

void phexdump_line(unsigned long address, unsigned char *data, int count){
    int i;

    printf("%08lX | ", address);
    for(i = 0; i < HEXDUMP_WIDTH; i++){
        if(i < count){
            phexdump_printhex(data[i]);
        }else{
            printf("   ");
        }
        if((i>0) && (i<(HEXDUMP_WIDTH-1)) && ((i%8) == 7)){
            printf("| ");
        }
    }

    printf("| ");

    for(i = 0; i < HEXDUMP_WIDTH; i++){
        if(i < count){
            phexdump_printchar(data[i]);
        }else{
            printf(" ");
        }
        if((i>0) && (i<(HEXDUMP_WIDTH-1)) && ((i%8) == 7)){
            printf(" ");
        }
    }

    printf(" |\n");
}

void phexdump_printchar(unsigned char ch){
    if((ch > 31) && (ch < 127)){
        printf("%c", ch);
    }else{
        printf(TG"."T0);
    }
}

void phexdump_printhex(unsigned char ch){
    printf("%02X ", ch);
}

