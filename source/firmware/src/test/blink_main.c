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

#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "types.h"
#include "driver/led.h"

void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
}

void my_delay(unsigned long delay)
{

	while (delay) {
		delay--;
		__asm("nop");
	}
}

void led_on(int id)
{
	switch (id) {
	case 0:
		LED_RED_ON();
		break;
	case 1:
		LED_GREEN_ON();
		break;
	case 2:
		LED_BLUE_ON();
		break;
	case 3:
		LED_ORANGE_ON();
		break;
	}
}

void led_off(int id)
{
	switch (id) {
	case 0:
		LED_RED_OFF();
		break;
	case 1:
		LED_GREEN_OFF();
		break;
	case 2:
		LED_BLUE_OFF();
		break;
	case 3:
		LED_ORANGE_OFF();
		break;
	}
}

int main(void)
{
	int i, j, led_id;

	system_init();
	led_init();

	led_id = 0;

	while (1) {
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 125; i++) {
				led_on(led_id);
				my_delay(50 * j);
				led_off(led_id);
				my_delay(1200 - 50 * j);
			}
		}
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 125; i++) {
				led_off(led_id);
				my_delay(200 + 50 * j);
				led_on(led_id);
				my_delay(1000 - 50 * j);
			}
		}
		led_off(led_id);
		led_id++;
		led_id %= 4;
	}
}
