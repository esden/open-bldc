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
 * @file   32bit_tim_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:52:28 2010
 *
 * @brief  @todo document
 *
 * @todo document
 */

#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>

#include "types.h"
#include "driver/led.h"
#include "test/32bit_tim_ref_tim.h"
#include "driver/32bit_tim.h"

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
 * @param delay @todo document
 */
void my_delay(unsigned long delay)
{

	while (delay) {
		delay--;
	}
}

/**
 * @todo document
 *
 * @return @todo document
 */
int main(void)
{
	int dir = 1;

	system_init();
	led_init();
	test_tim_init();
	tim_init();

	while (1) {

		if (dir > 0) {
			test_tim_update(test_tim_freq + 1);
		} else if (dir < 0) {
			test_tim_update(test_tim_freq - 1);
		}
		if (test_tim_freq <= 10) {
			dir = 1;
		} else if (test_tim_freq >= 50000) {
			dir = -1;
		}
		my_delay(1000);
		//LED_GREEN_TOGGLE();
	}
}
