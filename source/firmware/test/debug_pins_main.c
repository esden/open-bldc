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

#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "types.h"
#include "driver/mcu.h"
#include "driver/led.h"
#include "driver/debug_pins.h"

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
 * Turn on LED and debug pin by id.
 *
 * @param id LED and debug pin id
 */
static void dp_on(int id)
{
	switch (id) {
	case 0:
		ON(LED_RED);
		ON(DP_EXT_SCL);
		break;
	case 1:
		ON(LED_GREEN);
		ON(DP_EXT_SDA);
		break;
	case 2:
		ON(LED_BLUE);
		ON(DP_ENC_A);
		break;
	case 3:
		ON(LED_ORANGE);
		ON(DP_ENC_B);
		break;
	}
}

/**
 * Turn off LED and debug pin by id
 *
 * @param id LED and debug pin id
 */
static void dp_off(int id)
{
	switch (id) {
	case 0:
		OFF(LED_RED);
		OFF(DP_EXT_SCL);
		break;
	case 1:
		OFF(LED_GREEN);
		OFF(DP_EXT_SDA);
		break;
	case 2:
		OFF(LED_BLUE);
		OFF(DP_ENC_A);
		break;
	case 3:
		OFF(LED_ORANGE);
		OFF(DP_ENC_B);
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
	int i, j, dp_id;

	mcu_init();
	led_init();
	debug_pins_init();

	dp_id = 0;

	while (true) {
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 125; i++) {
				dp_on(dp_id);
				my_delay((unsigned long)(50 * j));
				dp_off(dp_id);
				my_delay((unsigned long)(1200 - 50 * j));
			}
		}
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 125; i++) {
				dp_off(dp_id);
				my_delay((unsigned long)(200 + 50 * j));
				dp_on(dp_id);
				my_delay((unsigned long)(1000 - 50 * j));
			}
		}
		dp_off(dp_id);
		dp_id++;
		dp_id %= 4;
	}
}
