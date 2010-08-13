/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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

#include <cmsis/stm32.h>

#include "types.h"
#include "sys_tick.h"

#include "driver/led.h"

u32 sys_tick_global_counter = 0;

void sys_tick_init(void)
{
	/* Setup SysTick Timer for 1uSec Interrupts */
	SysTick_Config(72000000 / 10000);
}

u32 sys_tick_get_timer(void)
{
	return sys_tick_global_counter;
}

int sys_tick_check_timer(u32 timer, u32 time)
{
	if((sys_tick_global_counter - timer) > time) {
		return 1;
	}else{
		return 0;
	}
}

void sys_tick_handler(void)
{
	sys_tick_global_counter++;
}
