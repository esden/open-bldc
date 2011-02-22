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
 * @file   cpu_load_process.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 24 16:20:19 2010
 *
 * @brief  Calculate and report current CPU load.
 *
 * This process is being called in every main loop cycle and incrementing a
 * counter that is reporting amount of main loop cycles in a second. This way it
 * is possible to read out the current load on the STM32.
 */

#include "config.h"

#include "cpu_load_process.h"

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "libopencm3/stm32/gpio.h"

#include "gprot.h"
#include "driver/sys_tick.h"
#include "driver/led.h"

/**
 * Infinite Impulse Response filter calculation
 */
#define CLP__IIR(VALUE, NEW_VALUE, IIR) \
	(((VALUE * IIR) + (NEW_VALUE)) / (IIR + 1))

/**
 * Internal state of the cpu load process.
 */
struct cpu_load_process_state {
	u32 cycles;
	u32 mean_cycles;
	u32 max_cycles;
	u32 min_cycles;
	u32 report_counter;
};

static struct cpu_load_process_state cpu_load_process_state; /**< Internal state instance */

static void cpu_load_process_soft_timer_callback(int id);

/**
 * Initialize internal state of the cpu load process.
 */
void cpu_load_process_init()
{
	(void)gpc_setup_reg(GPROT_CPU_LOAD, (u16 *)&cpu_load_process_state.mean_cycles);
	(void)gpc_setup_reg(GPROT_CPU_LOAD_MAX, (u16 *)&cpu_load_process_state.max_cycles);
	(void)gpc_setup_reg(GPROT_CPU_LOAD_MIN, (u16 *)&cpu_load_process_state.min_cycles);
	(void)sys_tick_timer_register(cpu_load_process_soft_timer_callback,
				CLP__TIME_BASE);
	cpu_load_process_reset();
}

/**
 * Reset the cpu load process
 */
void cpu_load_process_reset()
{
	cpu_load_process_state.cycles = 0;
	cpu_load_process_state.mean_cycles = 0;
	cpu_load_process_state.max_cycles = 0;
	cpu_load_process_state.min_cycles = -1;
	cpu_load_process_state.report_counter = CLP__REPORT_DIVIDER;
}

/**
 * Main periodic body of the cpu load process.
 */
void run_cpu_load_process()
{
	cpu_load_process_state.cycles++;
}

/**
 * Time reference software timer callback function.
 */
void cpu_load_process_soft_timer_callback(int id)
{

	id = id;

	if(cpu_load_process_state.cycles > cpu_load_process_state.max_cycles)
		cpu_load_process_state.max_cycles = cpu_load_process_state.cycles;
	else if(cpu_load_process_state.cycles < cpu_load_process_state.min_cycles)
		cpu_load_process_state.min_cycles = cpu_load_process_state.cycles;

	if(cpu_load_process_state.mean_cycles == 0){
		cpu_load_process_state.mean_cycles = cpu_load_process_state.cycles;
	} else {
		cpu_load_process_state.mean_cycles = CLP__IIR(cpu_load_process_state.mean_cycles,
							cpu_load_process_state.cycles,
							CLP__IIR_VALUE);
	}
	cpu_load_process_state.cycles = 0;

	if(cpu_load_process_state.report_counter == 0){
		cpu_load_process_state.report_counter = CLP__REPORT_DIVIDER;
		(void)gpc_register_touched(GPROT_CPU_LOAD);
		(void)gpc_register_touched(GPROT_CPU_LOAD_MAX);
		(void)gpc_register_touched(GPROT_CPU_LOAD_MIN);
	} else {
		cpu_load_process_state.report_counter--;
	}
}
