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
 * @file   rgb_fade_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:42:06 2010
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
#include "led_fade.h"

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
 */
int main(void)
{
	int i, d, on;

	system_init();
	led_init();
	led_fade_init();

	i = 5;
	d = 1;
	on = 0;
	while (1) {
		i += d;
		if (i == 250)
			d = -1;
		if (i == 5) {
			d = 1;
			led_fade_set_orange(0);
			led_fade_set_red(0);
			led_fade_set_green(0);
			led_fade_set_blue(0);
			on++;
			if (on == 4)
				on = 0;
		}
		switch (on) {
		case 0:
			led_fade_set_orange(i);
			break;
		case 1:
			led_fade_set_red(i);
			break;
		case 2:
			led_fade_set_green(i);
			break;
		case 3:
			led_fade_set_blue(i);
			break;
		}
		my_delay(50000);
	}
}
