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

/**
 * @file   mc_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 24 16:20:01 2010
 *
 * @brief  Motor controller main file.
 *
 * Implements the main() function of the motor controller.
 */

#include <libopencm3/stm32/rcc.h>

#include "types.h"
#include "driver/led.h"
#include "gprot.h"
#include "driver/usart.h"
#include "driver/adc.h"
#include "driver/sys_tick.h"
#include "driver/bemf_hardware_detect.h"
#include "driver/debug_pins.h"
#include "cpu_load_process.h"
#include "pwm/pwm.h"
#include "comm_tim.h"
#include "comm_process.h"
#include "sensor_process.h"
#include "control_process.h"

/**
 * Running in demo mode flag
 */
bool demo;

/**
 * Initialize STM32 system specific subsystems.
 */
static void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

/**
 * Main function of the motor controller.
 */
int main(void)
{
	int demo_counter;
	int demo_dir;

	system_init();
	led_init();
	//debug_pins_init();
	gprot_init();
	usart_init();
	sys_tick_init();
	cpu_load_process_init();
	comm_process_init();
	//sensor_process_init();
	//adc_init();
	pwm_init();
	comm_tim_init();
	control_process_init();
	bemf_hd_init();

	demo_counter = 500;
	demo_dir = 1;
	demo = false;

	while (true) {
		run_cpu_load_process();

		if (*comm_process_trigger) {
			*comm_process_trigger = false;
			run_comm_process();
		}

		run_control_process();

		//if (*sensor_process_trigger) {
		//	*sensor_process_trigger = false;
		//	run_sensor_process();
		//}

		if (demo) {
			if (demo_counter == 0) {
				demo_counter = 300;
				pwm_val += demo_dir;
				if (pwm_val > 2000) {
					demo_dir = -1;
				}

				if (pwm_val < 500) {
					demo_dir = 1;
				}
			} else {
				demo_counter--;
			}
		}
	}
}
