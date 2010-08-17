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
 * @file   sys_tick_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:41:29 2010
 *
 * @brief  @todo document
 *
 * @todo document
 */

#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "types.h"
#include "driver/led.h"
#include "driver/sys_tick.h"

/**
 * @todo document
 *
 */
void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
}

/**
 * @todo document
 *
 */
void sys_tick_timer_callback(void)
{
	LED_ORANGE_TOGGLE();
}

/**
 * @todo document
 *
 */
int main(void)
{
	u32 timer;

	system_init();
	led_init();
	sys_tick_init();

	sys_tick_timer_register(sys_tick_timer_callback, 5000);

	while (1) {
		timer = sys_tick_get_timer();
		while(!sys_tick_check_timer(timer, 500)){
			__asm("nop");
		}
		LED_RED_TOGGLE();
	}
}
