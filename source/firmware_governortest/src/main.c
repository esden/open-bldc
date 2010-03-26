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

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "led.h"
#include "gprot.h"
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
	//int i,j;
	u16 test_counter;

	system_init();
	led_init();
	gprot_init();
	usart_init();

	test_counter=0;
	gpc_setup_reg(5, &test_counter);

	while(1){
		my_delay(500000);
		test_counter++;
		gpc_register_touched(5);
/*
		for(j=0; j<20; j++){
			for(i=0; i< 125; i++){
				LED_ORANGE_ON();
				my_delay(50*j);
				LED_ORANGE_OFF();
				my_delay(1200-50*j);
			}
		}
		for(j=0; j<20; j++){
			for(i=0; i< 125; i++){
				LED_ORANGE_OFF();
				my_delay(200+50*j);
				LED_ORANGE_ON();
				my_delay(1000-50*j);
			}
		}
*/
	}
}
