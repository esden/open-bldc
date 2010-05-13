/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2009-2010 by Piotr Esden-Tempski <piotr@esden.net>
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
#include <stm32/tim.h>

#include "types.h"
#include "led.h"
#include "gprot.h"
#include "usart.h"
#include "adc.h"
#include "pwm.h"
#include "comm_tim.h"
#include "comm_process.h"
#include "sensor_process.h"
#include "control_process.h"

void system_init(void){
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
}

int main(void){

	system_init();
	led_init();
	gprot_init();
	usart_init();
	comm_process_init();
	sensor_process_init();
	adc_init();
	pwm_init();
	comm_tim_init();
	control_process_init();

	while(1){
		if(adc_new_data_trigger){
			adc_new_data_trigger = false;
			run_sensor_process();
			run_comm_process();
		}
		if(comm_tim_trigger){
			comm_tim_trigger = false;
			run_control_process();
		}
	}
}
