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

/*
 * Here you find the implementation of the Governor PC software protocol.
 */

#include <stm32/gpio.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "gprot_test_governor.h"
#include "driver/led.h"
#include "driver/usart.h"

void gprot_trigger_output(void *data);
void gprot_register_changed(void *data, u8 addr);

u16 test_regs[32];

/* Function implementations */
void gprot_init()
{
	int i;

	gpc_init(gprot_trigger_output, 0, gprot_register_changed, 0);

	for (i = 0; i < 32; i++) {
		test_regs[i] = i * 3;
		if (gpc_setup_reg(i, &test_regs[i])) {
			LED_RED_ON();
		}
	}

}

void gprot_trigger_output(void *data)
{
	data = data;
	LED_ORANGE_TOGGLE();
	usart_enable_send();
}

void gprot_register_changed(void *data, u8 addr)
{
	data = data;
}
