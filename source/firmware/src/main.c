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

#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>

#include "led.h"
#include "usart.h"

void system_init(void){
    /* Initialize the microcontroller system. Initialize clocks. */
    SystemInit();
}

void my_delay(unsigned long delay ){

    while(delay){
        delay--;
    }
}

int main(void){

    system_init();
    led_init();
    usart_init();

    while(1){
        LED_ORANGE_ON();
        my_delay(1000000);
        LED_ORANGE_OFF();
        my_delay(1000000);
        LED_GREEN_ON();
        my_delay(1000000);
        LED_GREEN_OFF();
        my_delay(1000000);
        LED_BLUE_ON();
        my_delay(1000000);
        LED_BLUE_OFF();
        my_delay(1000000);
    }
}
