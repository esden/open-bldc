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
 */
/**
 * @file   gprot_test_governor.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:43:25 2010
 *
 * @brief  Governor application specific implemetation used for the governor test
 *
 */

#include <libopenstm32/gpio.h>

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "gprot_test_governor.h"
#include "driver/led.h"
#include "driver/usart.h"

static void gprot_trigger_output(void *data);
static void gprot_register_changed(void *data, u8 addr);

/**
 * Test governor registers.
 */
static u16 test_regs[32];

/* Function implementations */
/**
 * Initialize the Governor protocol subsystem.
 */
void gprot_init()
{
	int i;

	(void)gpc_init(gprot_trigger_output, 0, gprot_register_changed, 0);

	for (i = 0; i < 32; i++) {
		test_regs[i] = (u16)(i * 3);
		if (gpc_setup_reg((u8)i, &test_regs[i]) != 0) {
			ON(LED_RED);
		}
	}

}

/**
 * Is called by libgovernor when it wants to send data out.
 *
 * In our case we enable the tx interrupt of the usart driver here.
 *
 * @param data Passthrough data of the callback. (ignored here)
 */
void gprot_trigger_output(void *data)
{
	data = data;
	TOGGLE(LED_ORANGE);
	usart_enable_send();
}

/**
 * Callback from libgovernor indicating a change in a register.
 *
 * @param data Passthrough data to the callback.
 * @param addr Address of the register that changed.
 */
void gprot_register_changed(void *data, u8 addr)
{
	data = data;
	addr = addr;
}
