/*
 * Open-BLDC - Open BruschLess DC Motor Controller
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

#include <stm32/tim.h>

#include "adc.h"

#include "soft_timer.h"

int comm_timer = 0;
volatile uint32_t comm_timer_reload = 72*2*750*2;
int cnt = 100;
volatile uint8_t soft_comm = 1;

void sys_tick_handler(void){
    /* generate a TIM1 COM event */
    //TIM_GenerateEvent(TIM1, TIM_EventSource_COM | TIM_EventSource_Update);
    if(soft_comm)
        TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
    //adc_val[((31-DMA_GetCurrDataCounter(DMA1_Channel1))%32)]|=0xF000;
#if 0
    if(led_state){
	    GPIOC->BRR |= 0x00001000;
	    led_state = 0;
    }else{
	    GPIOC->BSRR |= 0x00001000;
	    led_state = 1;
    }
#endif
}
