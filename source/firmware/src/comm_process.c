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

#include "types.h"

#include "led.h"
#include "comm_process.h"

#include "sensor_process.h"

struct comm_process_state {
	volatile bool rising;
	int hold_off;
	int pwm_count;
	bool closed_loop;
};

struct comm_process_state comm_process_state;
struct comm_params comm_params;

void comm_process_init(void)
{
	comm_process_state.rising = true;
	comm_process_state.hold_off = 2;
	comm_process_state.pwm_count = 0;
	comm_process_state.closed_loop = false;

	comm_params.spark_advance = 0;
	comm_params.direct_cutoff = 4000;
	comm_params.iir = 32;
}

void comm_process_reset(void)
{
	comm_process_state.pwm_count = 0;
}

void comm_process_config(bool rising){
	comm_process_state.rising = rising;
}

void comm_process_config_and_reset(bool rising){
	comm_process_state.rising = rising;
	comm_process_state.pwm_count = 0;
}

void comm_process_closed_loop_on(void)
{
	comm_process_state.closed_loop = true;
}

void comm_process_closed_loop_off(void)
{
	comm_process_state.closed_loop = false;
}

void run_comm_process(void)
{
	LED_RED_ON();
	if(comm_process_state.pwm_count < comm_process_state.hold_off){
		comm_process_state.pwm_count++;
		return;
	}

	if(comm_process_state.rising){
		if(sensors.phase_voltage >= sensors.half_battery_voltage){
			LED_ORANGE_ON();
		}else{
			LED_ORANGE_OFF();
		}
	}else{
		if(sensors.phase_voltage <= sensors.half_battery_voltage){
			LED_ORANGE_OFF();
		}else{
			LED_ORANGE_ON();
		}
	}

	LED_RED_OFF();
}
