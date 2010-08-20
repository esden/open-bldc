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
 * @file   cp_spinup_2step.c
 * @author Tobias Fuchs <twh.fuchs@gmail.com>
 * @date   Thu Aug 19 15:59:42 2010
 *
 * @brief  Two-step spin up strategy
 *
 * Two-Step spin up strategy providing a control process callback
 * function for control state cps_spinup.
 * Consists two (private) delegate states fine_spinup and
 * coarse_spinup that are switched automatically.
 * This implementation is known to be disfunctional and will be
 * replaced by a working strategy soon.
 */

#include "config.h"

#include "cp_spinup.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

/**
 * Trigger source for spinup state.
 */
bool *control_process_spinup_trigger = &comm_tim_trigger;

/**
 * Internal process states for spinup callback.
 */
enum spinup_state {
	spinup_state_coarse=0,
	spinup_state_fine
};

/**
 * Internal process variables for spinup callback.
 */
struct spinup_process {
	int coarse_spinup_time;		   /**< Coarce spinup timer */
	int coarse_spinup_step;		   /**< Current coarce spinup step */
};
static struct spinup_process spinup_process;

/**
 *	Current internal state of the spin up process.
 */
enum spinup_state spinup_state;

enum control_process_cb_state control_process_fine_spinup_cb(struct control_process * cps);
enum control_process_cb_state control_process_coarse_spinup_cb(struct control_process * cps);

/**
 * Initialization of the spinup callback process, currently
 * redirecting to @ref cp_spinup_reset.
 */
void cp_spinup_init(void)
{
	cp_spinup_reset();
}

/**
 * Reset function for the spinup callback process.
 * Sets spin up state to spinup_state_coarse and resets
 * internal countdowns.
 */
void cp_spinup_reset(void)
{
	spinup_state = spinup_state_coarse;
	spinup_process.coarse_spinup_time = 0;
	spinup_process.coarse_spinup_step =
	    CONTROL_PROCESS_COARSE_MAX_SPINUP_STEP;
}

/**
 * Internal spinup process for the coarse spin up
 * phase.
 */
enum control_process_cb_state
control_process_coarse_spinup_cb(struct control_process * cps) {
	if (spinup_process.coarse_spinup_step > 0) {
		if (spinup_process.coarse_spinup_time == 0) {
			comm_tim_trigger_comm_once = true;

			spinup_process.coarse_spinup_time = spinup_process.coarse_spinup_step;

			if ((spinup_process.coarse_spinup_step /
					 CONTROL_PROCESS_COARSE_SPINUP_DEC_DIV) == 0) {
				spinup_process.coarse_spinup_step--;
			}
			else {
				spinup_process.coarse_spinup_step =
						 spinup_process.coarse_spinup_step -
						(spinup_process.coarse_spinup_step
						 /
						 CONTROL_PROCESS_COARSE_SPINUP_DEC_DIV);
			}
		}
		else {
			spinup_process.coarse_spinup_time--;
		}
	}
	else {
		comm_tim_trigger_comm = true;
		spinup_state  = spinup_state_fine;
	}
	return cps_cb_continue;
}

/**
 * Internal spinup process for the fine spin up
 * phase.
 */
enum control_process_cb_state
control_process_fine_spinup_cb(struct control_process * cps)
{
	if (comm_data.bemf_crossing_detected) {
		comm_data.bemf_crossing_detected = false;
		cps->bemf_crossing_counter++;
		cps->bemf_lost_crossing_counter = 0;
	}
	else {
		cps->bemf_crossing_counter = 0;
		cps->bemf_lost_crossing_counter++;
	}

	if ((cps->bemf_crossing_counter > 2) &&
			(comm_tim_data.freq < 30000) &&
			(comm_data.in_range_counter > 2)) {
		cps->state = cps_spinning;
		LED_RED_ON();
		return cps_cb_resume_control;
	}

	comm_tim_data.freq =
			comm_tim_data.freq -
			(comm_tim_data.freq / CONTROL_PROCESS_SPINUP_DEC_DIV);
	if (comm_tim_data.freq < 10000) {
		return cps_cb_exit_control;
	}
	return cps_cb_continue;
}

/**
 * Callback function to be hooked as handler for state
 * cps_spinup in control_process.c.
 * Implements a simplicistic 2-step spin up (coarse and
 * fine).
 * Coarse spin up process @ref control_process_coarse_spinup_cb()
 * induces a transition to fine spin up by setting the local
 * state variable #spinup_state from spinup_state_coarse
 * to spinup_state_fine.
 */
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

/**
 * Callback function called before entering control
 * process state cps_spinup.
 */
enum control_process_cb_state
control_process_spinup_state_in_cb(struct control_process * cps) {
	return cps_cb_continue;
}

/**
 * Callback function called after leaving control
 * process state cps_spinup, before entering the
 * next control process state.
 */
enum control_process_cb_state
control_process_spinup_state_out_cb(struct control_process * cps) {
	return cps_cb_continue;
}

