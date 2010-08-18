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

/**
 * @file   sys_tick.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 02:02:50 2010
 *
 * @brief  Sys Tick based soft timer driver implementation
 *
 * Sys Tick is a part of the Cortex M3 core and can be used as a system timer.
 * This implementation uses it as a coarce soft timer source.
 */

#include <cmsis/stm32.h>

#include "types.h"
#include "sys_tick.h"

#include "driver/led.h"

/**
 * Amount of available Sys Tick based soft timer slots.
 *
 * @todo move to global config header
 */
#define SYS_TICK_TIMER_NUM 5

/**
 * Private global sys tick counter.
 */
u32 sys_tick_global_counter = 0;

/**
 * Represents one Sys Tick based soft timer.
 */
struct sys_tick_timer {
	sys_tick_timer_callback_t callback; /**< Callback function pointer */
	u32 start_time; /**< Start timestamp of the timer */
	u32 delta_time; /**< Duration of the timer */
};

/**
 * Instances of available Sys Tick timer slots.
 */
struct sys_tick_timer sys_tick_timers[SYS_TICK_TIMER_NUM];

/**
 * Initialize Sys Tick peripheral and the soft timer slots.
 */
void sys_tick_init(void)
{
	int i;

	/* Setup SysTick Timer for 1uSec Interrupts */
	SysTick_Config(72000000 / 10000);

	for(i = 0; i < SYS_TICK_TIMER_NUM; i++){
		sys_tick_timers[i].callback = 0;
		sys_tick_timers[i].start_time = 0;
		sys_tick_timers[i].delta_time = 0;
	}
}

/**
 * Get a new timer.
 *
 * Use this if you want busy wait for some time. To detect the new time use
 * @ref sys_tick_check_timer()
 *
 * @return Timer ID
 */
u32 sys_tick_get_timer(void)
{
	return sys_tick_global_counter;
}

/**
 * Check actively if a certain time elapsed.
 *
 * @param timer Timer aquired using @ref sys_tick_get_timer()
 * @param time Time delay to check against.
 *
 * @return 0 if the time did not elapse yet, 1 if the time elapsed.
 */
int sys_tick_check_timer(u32 timer, u32 time)
{
	if((sys_tick_global_counter - timer) > time) {
		return 1;
	}else{
		return 0;
	}
}

/**
 * Sys Tick interrupt handler.
 */
void sys_tick_handler(void)
{
	int i;

	sys_tick_global_counter++;

	for(i = 0; i < SYS_TICK_TIMER_NUM; i++){
		if(sys_tick_timers[i].callback &&
			sys_tick_check_timer(
				sys_tick_timers[i].start_time,
				sys_tick_timers[i].delta_time)) {
			sys_tick_timers[i].start_time = sys_tick_global_counter;
			sys_tick_timers[i].callback();
		}
	}
}

/**
 * Register a soft timer callback.
 *
 * @param callback Callback function that should be called after a time elapses.
 * @param time Delay to wait for.
 *
 * @return ID of the soft timer.
 */
int sys_tick_timer_register(sys_tick_timer_callback_t callback, u32 time)
{
	int i;
	u32 start_time = sys_tick_global_counter;

	for(i = 0; i < SYS_TICK_TIMER_NUM; i++){
		if(!sys_tick_timers[i].callback){
			sys_tick_timers[i].callback = callback;
			sys_tick_timers[i].start_time = start_time;
			sys_tick_timers[i].delta_time = time;
			return 0;
		}
	}

	return 1;
}
