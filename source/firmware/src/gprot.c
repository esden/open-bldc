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
#include "usart.h"
#include "pwm.h"
#include "comm_tim.h"
#include "adc.h"

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

#define GPROT_FLAG_PWM_COMM (1 << 0)
#define GPROT_FLAG_COMM_TIM (1 << 1)
#define GPROT_FLAG_ADC_COMM (1 << 2)

u16 gprot_dummy_val;
u16 gprot_flag_reg;

u16 *gprot_registers[32];

/*
 * Every bit in this map represents a register, if it is set to 1 the content of
 * the register gets sent to the PC everytime it gets poked.
 */
u32 gprot_cont_reg_map;

/*
 * Every bit in this map represents a register, the register contents which bits
 * are set to 1 get sent to the PC asap.
 */
u32 gprot_send_reg_map;

enum gprot_read_states {
    GPROT_R_ADDR,
    GPROT_R_LSB,
    GPROT_R_MSB
};

u8 gprot_read_addr;
enum gprot_read_states gprot_r_state = GPROT_R_ADDR;

/* Private function declarations */
void gprot_update_flags(void);

/* Function implementations */
void gprot_init(){
    int i;

    gprot_flag_reg = 0;
    gprot_dummy_val = 0;
    gprot_w_state = GPROT_W_IDLE;
    gprot_r_state = GPROT_R_ADDR;

    for(i=0; i<32; i++){
        gprot_registers[i] = &gprot_dummy_val;
    }

    gprot_cont_reg_map = 0;
    gprot_send_reg_map = 0;

    gprot_conf_reg(0, &gprot_flag_reg);
}

u8 gprot_handle(u8 byte){
    static u8 write_addr;

    switch(gprot_w_state){
    case GPROT_W_IDLE:
        if((byte & GPROT_MODE_RW_MSK) == GPROT_MODE_WRITE){
            //LED_RED_TOGGLE();
            write_addr = byte & GPROT_ADDR_MSK;
            gprot_w_state = GPROT_W_VAL_LSB;
            return 0;
        }else{
            //LED_GREEN_TOGGLE();
            if((byte & GPROT_MODE_PC_MSK) == GPROT_MODE_PEEK){
                gprot_poke_reg(byte & GPROT_ADDR_MSK);
                return 1;
            }else{
                gprot_toggle_cont_reg(byte & GPROT_ADDR_MSK);
                return 0;
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
        if(write_addr == 0){
            gprot_update_flags();
        }
        return 0;
        break;
    }

    return 0;
}

s16 gprot_get_byte(){

    switch(gprot_r_state){
    case GPROT_R_ADDR:
        gprot_r_state = GPROT_R_LSB;
        gprot_read_addr = gprot_get_next_addr();
        if(gprot_read_addr > 31){
            return -1;
        }else{
            return gprot_read_addr;
        }
        break;
    case GPROT_R_LSB:
        gprot_r_state = GPROT_R_MSB;
        return (*gprot_registers[gprot_read_addr]) & 0xFF;
        break;
    case GPROT_R_MSB:
        if(!gprot_send_reg_map){
            usart_disable_send();
        }
        gprot_r_state = GPROT_R_ADDR;
        return (*gprot_registers[gprot_read_addr]) >> 8;
        break;
    }

    return -2;
}

void gprot_conf_reg(u8 addr, u16 *value){
    gprot_registers[addr] = value;
}

void gprot_toggle_cont_reg(u8 addr){
    gprot_cont_reg_map ^= (1 << addr);
}

void gprot_poke_cont_reg(u8 addr){
    if(gprot_send_reg_map |= gprot_cont_reg_map & (1 << addr)){
        usart_enable_send();
    }
}

void gprot_poke_reg(u8 addr){
    gprot_send_reg_map |= 1 << addr;
    usart_enable_send();
}

u8 gprot_get_next_addr(void){
    u8 addr = 0;

    if(!gprot_send_reg_map){
        LED_RED_TOGGLE();
        return 33; /* max addr + 1 to indicate empty map */
    }

    /* calculate the next register address to send obviously the priority of
     * registers is from lsb to msb, so register 0 has the highest priority and
     * 31 the lowest.
     */
    for(addr = 0; !((gprot_send_reg_map >> addr) & 1) && addr < 32; addr++){}

    /* Clear the corresponding register bit using XOR as we hope that the bit is
     * always on when we are here
     */
    gprot_send_reg_map ^= (1 << addr);

    return addr;
}

void gprot_update_flags(void){
    if(gprot_flag_reg & GPROT_FLAG_PWM_COMM){
        pwm_comm();
        return;
    }

    if(gprot_flag_reg & GPROT_FLAG_COMM_TIM){
        comm_tim_on();
    }else{
        comm_tim_off();
    }

    if(gprot_flag_reg & GPROT_FLAG_ADC_COMM){
        adc_comm = 1;
    }else{
        adc_comm = 0;
    }

    /* add other flags here (up to 16) */
}
