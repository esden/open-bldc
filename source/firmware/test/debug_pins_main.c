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

#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "types.h"
#include "driver/led.h"
#include "driver/debug_pins.h"

/**
 * Initialize the STM32
 */
static void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
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
 * Turn on LED and debug pin by id.
 *
 * @param id LED and debug pin id
 */
static void dp_on(int id)
{
	switch (id) {
	case 0:
		LED_RED_ON();
		DP_EXT_SCL_ON();
		break;
	case 1:
		LED_GREEN_ON();
		DP_EXT_SDA_ON();
		break;
	case 2:
		LED_BLUE_ON();
		DP_ENC_A_ON();
		break;
	case 3:
		LED_ORANGE_ON();
		DP_ENC_B_ON();
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
		LED_RED_OFF();
		DP_EXT_SCL_OFF();
		break;
	case 1:
		LED_GREEN_OFF();
		DP_EXT_SDA_OFF();
		break;
	case 2:
		LED_BLUE_OFF();
		DP_ENC_A_OFF();
		break;
	case 3:
		LED_ORANGE_OFF();
		DP_ENC_B_OFF();
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

	system_init();
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
