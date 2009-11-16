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

#include <stdlib.h>

#include "types.h"
#include "serial.h"
#include "protocol.h"

int gp_set(u8 addr, u16 val){
    u8 dat[3];

    dat[0] = addr | GP_MODE_WRITE;
    dat[1] = val & 0xFF;
    dat[2] = val >> 8;

    return serial_write(dat, 3);
}

int gp_get(u8 addr, u16 *val){
    int ret;
    u8 out = addr | GP_MODE_READ | GP_MODE_PEEK;
    u8 in[3];

    if((ret = serial_write(&out, 1)) < 0){
        return ret;
    }

    ret = serial_read(in, 3);
    if(ret < 0){
        return ret;
    }

    *val = in[1] | ((u16)in[2]) << 8;

    if(in[0] > 31){
        return -1;
    }

    return 0;
}

int gp_conf_cont_read_slot(u8 addr, u8 slot){
    u8 dat[2];

    dat[0] = addr | GP_MODE_READ | GP_MODE_CONT;
    dat[1] = slot;

    return serial_write(dat, 2);
}
