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

/**
 * @file   governor_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:46:21 2010
 *
 * @brief  Governor protocol test implementation
 *
 */
#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "driver/led.h"
#include "test/gprot_test_governor.h"
#include "driver/usart.h"

/**
 * Initialize STM32
 */
void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
}

/**
 * Crude delay implementation.
 *
 * Burn some MCU cycles.
 *
 * @param delay "time" delay
 */
void my_delay(unsigned long delay)
{

	while (delay) {
		delay--;
	}
}

/**
 * Governor protocol test main function
 */
int main(void)
{
	u16 test_counter;

	system_init();
	led_init();
	gprot_init();
	usart_init();

	test_counter = 0;
	gpc_setup_reg(5, &test_counter);

	while (1) {
		my_delay(500000);
		test_counter++;
		gpc_register_touched(5);
	}
}
