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

#include "driver/led.h"
#include "comm_tim.h"
#include "comm_process.h"

#include "sensor_process.h"

/**
 * Internal state of the comm process struct.
 */
struct comm_process_state {
	volatile bool rising;	/**< Waiting for rising or falling edge of BEMF */
	int pwm_count;		/**< PWM cycle counter in the current commutation */
	bool closed_loop;	/**< Running in closed loop control flag */
	u32 prev_phase_voltage; /**< Previous PWM cycle phase voltage memory */
};

struct comm_process_state comm_process_state;	/**< Internal state instance */
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
	comm_process_state.rising = true;
	comm_process_state.pwm_count = 0;
	comm_process_state.closed_loop = false;
	comm_process_state.prev_phase_voltage = 0;

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
	comm_process_state.pwm_count = 0;
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
	if (rising) {
		comm_process_state.prev_phase_voltage = 65535;
		sensors.phase_voltage = 65535;
	} else {
		comm_process_state.prev_phase_voltage = 0;
		sensors.phase_voltage = 0;
	}
	comm_process_state.pwm_count = 0;
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
 * Calculate the next commutation time
 */
void comm_process_calc_next_comm(void)
{
	s32 old_cycle_time = comm_tim_data.freq;
	s32 pwm_time = comm_tim_data.curr_time - comm_tim_data.prev_time;
	s32 bemf_rise = sensors.phase_voltage -
	    comm_process_state.prev_phase_voltage;
	s32 zero_value = sensors.half_battery_voltage -
	    comm_process_state.prev_phase_voltage;
	s32 half_cycle_time_adjust = (zero_value * pwm_time) / bemf_rise;
	u32 half_cycle_time = (comm_tim_data.prev_time -
			       comm_tim_data.last_capture_time) +
	    half_cycle_time_adjust;
	new_cycle_time = half_cycle_time * 2;

	if (new_cycle_time > (old_cycle_time + comm_params.direct_cutoff)) {
		new_cycle_time = old_cycle_time +
		    comm_params.direct_cutoff_slope;
		comm_data.in_range_counter = 0;
	} else if (new_cycle_time <
		   (old_cycle_time - comm_params.direct_cutoff)) {
		new_cycle_time = old_cycle_time -
		    comm_params.direct_cutoff_slope;
		comm_data.in_range_counter = 0;
	} else {
		comm_data.in_range_counter++;
	}

	if (comm_process_state.closed_loop) {
		new_cycle_time = ((old_cycle_time * comm_params.iir) +
				  (new_cycle_time +
				   comm_params.spark_advance)) /
		    (comm_params.iir + 1);
		comm_tim_data.freq = new_cycle_time;
		comm_tim_update_freq();
	}
}

/**
 * Main periodic body of the commutation process
 */
void run_comm_process(void)
{
	if (comm_process_state.pwm_count < comm_params.hold_off) {
		comm_process_state.pwm_count++;
		comm_process_state.prev_phase_voltage = sensors.phase_voltage;
		LED_ORANGE_OFF();
		return;
	}

	if ((sensors.phase_voltage > 500) &&
	    (sensors.phase_voltage < (0xFFF - 500))) {
		if (comm_process_state.rising) {
			if ((comm_process_state.prev_phase_voltage <
			     sensors.phase_voltage)
			    && (sensors.phase_voltage >=
				sensors.half_battery_voltage)
			    && (!comm_data.bemf_crossing_detected)) {
				comm_process_calc_next_comm();
				comm_data.bemf_crossing_detected = true;
				LED_ORANGE_ON();
			} else {
				LED_ORANGE_OFF();
			}
		} else {
			if ((comm_process_state.prev_phase_voltage >
			     sensors.phase_voltage)
			    && (sensors.phase_voltage <=
				sensors.half_battery_voltage)
			    && (!comm_data.bemf_crossing_detected)) {
				comm_process_calc_next_comm();
				comm_data.bemf_crossing_detected = true;
				LED_ORANGE_ON();
			} else {
				LED_ORANGE_OFF();
			}
		}
	} else {
		LED_ORANGE_OFF();
	}

	comm_process_state.prev_phase_voltage = sensors.phase_voltage;
}
