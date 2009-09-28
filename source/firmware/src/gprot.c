/*
 * Open-BLDC - Open BrushLess DC Motor Controller
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
 * Here you find the implementation of the Governor PC software protocol.
 */

#include <stm32/gpio.h>

#include "types.h"
#include "gprot.h"
#include "led.h"

enum gprot_write_states {
    GPROT_W_IDLE,
    GPROT_W_VAL_LSB,
    GPROT_W_VAL_MSB
};

enum gprot_write_states gprot_w_state = GPROT_W_IDLE;

#define GPROT_MODE_PC_MSK (1 << 6)
#define GPROT_MODE_PEEK 0
#define GPROT_MODE_CONT (1 << 6)

#define GPROT_MODE_RW_MSK (1 << 7)
#define GPROT_MODE_READ 0
#define GPROT_MODE_WRITE (1 << 7)

#define GPROT_ADDR_MSK 0x1F

u16 gprot_dummy_val;
u16 gprot_dummy_val1;

u16 *gprot_registers[32];

enum gprot_read_states {
    GPROT_R_ADDR,
    GPROT_R_LSB,
    GPROT_R_MSB,
    GPROT_R_FIN
};

u8 gprot_read_addr;
enum gprot_read_states gprot_r_state = GPROT_R_ADDR;

void gprot_init(){
    int i;

    gprot_dummy_val = 0;
    gprot_dummy_val1 = 0;
    gprot_w_state = GPROT_W_IDLE;
    gprot_r_state = GPROT_R_ADDR;

    for(i=0; i<32; i++){
        gprot_registers[i] = &gprot_dummy_val;
    }

    gprot_registers[1] = &gprot_dummy_val1;

}

u8 gprot_handle(u8 byte){
    static u8 write_addr;

    switch(gprot_w_state){
    case GPROT_W_IDLE:
        if((byte & GPROT_MODE_RW_MSK) == GPROT_MODE_WRITE){
            LED_BLUE_TOGGLE();
            write_addr = byte & GPROT_ADDR_MSK;
            gprot_w_state = GPROT_W_VAL_LSB;
            return 0;
        }else{
            LED_GREEN_TOGGLE();
            if((byte & GPROT_MODE_PC_MSK) == GPROT_MODE_PEEK){
                gprot_read_addr = byte & GPROT_ADDR_MSK;
                return 1;
            }else{
            }
        }
        break;
    case GPROT_W_VAL_LSB:
        *gprot_registers[write_addr] = byte;
        gprot_w_state = GPROT_W_VAL_MSB;
        return 0;
        break;
    case GPROT_W_VAL_MSB:
        *gprot_registers[write_addr] |= ((u16)byte) << 8;
        gprot_w_state = GPROT_W_IDLE;
        return 0;
        break;
    default:
        return 0;
        break;
    }

    return 0;
}

s16 gprot_get_byte(){

    switch(gprot_r_state){
    case GPROT_R_ADDR:
        gprot_r_state = GPROT_R_LSB;
        return gprot_read_addr;
        break;
    case GPROT_R_LSB:
        gprot_r_state = GPROT_R_MSB;
        return *gprot_registers[gprot_read_addr] & 0xFF;
        break;
    case GPROT_R_MSB:
        gprot_r_state = GPROT_R_FIN;
        return *gprot_registers[gprot_read_addr] >> 8;
        break;
    case GPROT_R_FIN:
        gprot_r_state = GPROT_R_ADDR;
        return -1;
    }

    return -2;
}
