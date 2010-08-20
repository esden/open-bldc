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
 * @file   spinup.c
 * @author Tobias Fuchs <twh.fuchs@gmail.com>
 * @date   Thu Aug 19 15:59:42 2010
 *
 * @brief  Two-step spin up strategy
 *
 * Two-Step spin up strategy providing a control process callback 
 * function for control state cps_spinup. 
 * Consists two (private) delegate states fine_spinup and 
 * coarse_spinup that are switched automatically. 
 */

#include "types.h"
#include "comm_tim.h"
#include "spinup.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"
#include "control_process.h"

enum spinup_state { 
	spinup_state_coarse=0, 
	spinup_state_fine
};

enum spinup_state spinup_state; 

enum control_process_cb_state control_process_fine_spinup_cb(struct control_process * cps);
enum control_process_cb_state control_process_coarse_spinup_cb(struct control_process * cps);

void spinup_reset(void)
{
	spinup_state = spinup_state_coarse; 
// TODO	cps->coarce_spinup_time = COARSE_COUNTDOWN;
}

enum control_process_cb_state
control_process_coarse_spinup_cb(struct control_process * cps) { 
	if (cps->coarce_spinup_step > 0) {
		if (cps->coarce_spinup_time == 0) {
			comm_tim_trigger_comm_once = true;

			cps->coarce_spinup_time = cps->coarce_spinup_step;

			if ((cps->coarce_spinup_step /
					 CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV) == 0) {
				cps->coarce_spinup_step--;
			} 
			else {
				cps->coarce_spinup_step =
						cps->coarce_spinup_step -
						(cps->coarce_spinup_step
						 /
						 CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV);
			}
		} 
		else {
			cps->coarce_spinup_time--;
		}
	} 
	else {
		comm_tim_trigger_comm = true;
		cps->state = cps_spinup;
	}
	return cps_cb_continue;
}

enum control_process_cb_state
control_process_fine_spinup_cb(struct control_process * cps) { 
	if (comm_data.bemf_crossing_detected) {
		comm_data.bemf_crossing_detected = false;
		cps->bemf_crossing_counter++;
		cps->bemf_lost_crossing_counter = 0;
	} else {
		cps->bemf_crossing_counter = 0;
		cps->bemf_lost_crossing_counter++;
	}

	if ((cps->bemf_crossing_counter > 2) &&
			(comm_tim_data.freq < 30000) &&
			(comm_data.in_range_counter > 2)) {
		comm_process_closed_loop_on();
		cps->state = cps_spinning;
		LED_RED_ON();
		return cps_cb_break;
	}

	comm_tim_data.freq =
			comm_tim_data.freq -
			(comm_tim_data.freq / CONTROL_PROCESS_SPINUP_DEC_DIV);
	if (comm_tim_data.freq < 10000) {
		control_process_kill();
	}
	return cps_cb_continue;
}

enum control_process_cb_state
control_process_spinup_cb(struct control_process * cps) { 
	switch(spinup_state) { 
		case spinup_state_fine: 
			return control_process_fine_spinup_cb(cps); 
			break; 
		case spinup_state_coarse:
			return control_process_coarse_spinup_cb(cps); 
			break;
		default: 
			return cps_error; 
	}
}

