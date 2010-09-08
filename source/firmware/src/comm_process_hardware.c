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
 * @file   comm_process.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 02:06:02 2010
 *
 * @brief  Commutation control userspace process implementation.
 *
 * This process is being called periodically from the main while loop and
 * generates commutation events.
 */

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "gprot.h"
#include "driver/led.h"
#include "driver/bemf_hardware_detect.h"
#include "comm_tim.h"
#include "comm_process.h"

#include "sensor_process.h"

volatile bool *comm_process_trigger;

/**
 * Internal state of the comm process struct.
 */
struct comm_process_state {
	volatile bool rising;	/**< Waiting for rising or falling edge of BEMF */
	bool closed_loop;	/**< Running in closed loop control flag */
};

/**
 * Comm process parameters
 *
 * @todo clean up documentation
 */
struct comm_params {
	s16 spark_advance;	 /**< advance commutation relative to calculated time */
	u16 direct_cutoff;	 /**< distance from the last calc time that makes the new invalid */
	u16 direct_cutoff_slope; /**< what is the control slope when outside the direct control window */
	u16 iir;		 /**< IIR value for the commutation time */
	u16 hold_off;		 /**< how many bemf samples after a commutation should be dropped */
};

static struct comm_process_state comm_process_state;	/**< Internal state instance */
struct comm_data comm_data;			/**< Public data instance */
struct comm_params comm_params;			/**< Parameters */
s32 new_cycle_time;				/**< New commutation time
						  temporary variable, @todo
						  remove */

/**
 * Initialize commutation process
 */
void comm_process_init(void)
{

	gpc_setup_reg(GPROT_COMM_TIM_SPARK_ADVANCE_REG_ADDR,
		      (u16 *) & (comm_params.spark_advance));
	gpc_setup_reg(GPROT_COMM_TIM_DIRECT_CUTOFF_REG_ADDR,
		      &(comm_params.direct_cutoff));
	gpc_setup_reg(GPROT_COMM_TIM_IIR_POLE_REG_ADDR, &(comm_params.iir));

	comm_process_trigger = &bemf_hd_data.trigger;

	comm_process_state.rising = true;
	comm_process_state.closed_loop = false;

	comm_data.bemf_crossing_detected = false;
	comm_data.calculated_freq = 0;
	comm_data.in_range_counter = 0;

	comm_params.spark_advance = 0;
	comm_params.direct_cutoff = 10000;
	comm_params.direct_cutoff_slope = 20;
	comm_params.iir = 6;
	comm_params.hold_off = 1;
}

/**
 * Reset commutation process internal state.
 */
void comm_process_reset(void)
{

}

/**
 * Commutation process configuration
 *
 * @param rising true selects rising edge of bemf and false the falling edge of
 * the BEMF
 */
void comm_process_config(bool rising)
{
	comm_process_state.rising = rising;
}

/**
 * Reset and configure commutation process.
 *
 * @param rising true selects rising edge of BEMF and false the falling edge of
 * the BEMF
 */
void comm_process_config_and_reset(bool rising)
{
	comm_process_state.rising = rising;
}

/**
 * Switch on the closed loop control system
 */
void comm_process_closed_loop_on(void)
{
	comm_process_state.closed_loop = true;
}

/**
 * Switch off the closed loop control system
 */
void comm_process_closed_loop_off(void)
{
	comm_process_state.closed_loop = false;
}

/**
 * Main periodic body of the commutation process
 */
void run_comm_process(void)
{

}

/**
 * Do we have enough information for calculating commutation times?
 */
bool comm_process_ready(void)
{
	return false;
}
