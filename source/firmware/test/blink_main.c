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
 * @file   blink_main.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:47:20 2010
 *
 * @brief  Blink test implementation
 *
 */

#include <libopenstm32/rcc.h>
#include <libopenstm32/gpio.h>

#include "types.h"
#include "driver/led.h"

/**
 * Initialize the STM32
 */
static void system_init(void)
{
	/* Initialize clocks. */
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

/**
 * Crude delay implementation.
 *
 * Just burn some MCU cycles
 *
 * @param delay "time" to wait
 */
static void my_delay(unsigned long delay)
{

	while (delay != 0) {
		delay--;
		__asm("nop");
	}
}

/**
 * Turn on LED by id.
 *
 * @param id LED id
 */
static void led_on(int id)
{
	switch (id) {
	case 0:
		ON(LED_RED);
		break;
	case 1:
		ON(LED_GREEN);
		break;
	case 2:
		ON(LED_BLUE);
		break;
	case 3:
		ON(LED_ORANGE);
		break;
	}
}

/**
 * Turn off LED by id
 *
 * @param id LED id
 */
static void led_off(int id)
{
	switch (id) {
	case 0:
		OFF(LED_RED);
		break;
	case 1:
		OFF(LED_GREEN);
		break;
	case 2:
		OFF(LED_BLUE);
		break;
	case 3:
		OFF(LED_ORANGE);
		break;
	}
}

/**
 * Blink test main function
 *
 * @return Nothing really...
 */
int main(void)
{
	int i, j, led_id;

	system_init();
	led_init();

	led_id = 0;

	while (true) {
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 125; i++) {
				led_on(led_id);
				my_delay((unsigned long)(50 * j));
				led_off(led_id);
				my_delay((unsigned long)(1200 - 50 * j));
			}
		}
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 125; i++) {
				led_off(led_id);
				my_delay((unsigned long)(200 + 50 * j));
				led_on(led_id);
				my_delay((unsigned long)(1000 - 50 * j));
			}
		}
		led_off(led_id);
		led_id++;
		led_id %= 4;
	}
}
