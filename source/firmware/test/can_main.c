/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2009 - 2010 by Piotr Esden-Tempski <piotr@esden.net>
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
 * @file   blink_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:47:20 2010
 *
 * @brief  Blink test implementation
 *
 */

#include "config.h"

#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "types.h"
#include "driver/led.h"
#include "driver/can.h"
#include "driver/sys_tick.h"

/**
 * Initialize the STM32
 */
static void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
}

/**
 * Blink test main function
 *
 * @return Nothing really...
 */
int main(void)
{
	int timer;
	uint8_t data[1];

	system_init();
	led_init();
	sys_tick_init();
	can_init();

	timer = sys_tick_get_timer();

	data[0] = 10;

	while (true) {
		if (sys_tick_check_timer(timer, 10000)) {
			data[0]++;
			timer = sys_tick_get_timer();
#ifdef CAN_ADDR
			can_transmit(CAN_ADDR, data, 1);
#else
			can_transmit(CAN__DEFAULT_ADDR, data, 1);
#endif
		}
	}
}
