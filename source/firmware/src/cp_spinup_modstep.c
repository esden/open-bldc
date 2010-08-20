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
 * @file   cp_spinup_modstep.c
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

#include "cp_spinup.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

/**
 * Maximum (initial) value of the main spin up countdown.
 */
#define SPINUP_MODSTEP_COUNTER_MAX 60000
/**
 * Maximum (initial) value of the mod spin up countdown.
 */
#define SPINUP_MODSTEP_COUNTER_MOD_MAX 1000

/**
 * Trigger source for spinup state.
 */
bool *control_process_spinup_trigger = &comm_tim_trigger;

/**
 * Internal process variables for spinup callback.
 */
struct spinup_process {
	int counter;     /**< Main countdown variable for speedup */
	int mod_counter; /**< Mod countdown variable for speedup */
};
static struct spinup_process spinup_process;


/**
 * Initialization of the spinup callback process, currently
 * calls @ref cp_spinup_reset().
 */
void cp_spinup_init(void)
{
	cp_spinup_reset();
	control_process_register_cb(cps_spinup,
				    control_process_spinup_trigger,
				    control_process_spinup_cb,
				    control_process_spinup_state_in_cb,
				    control_process_spinup_state_out_cb);
}

/**
 * Reset function for the spinup callback process.
 * Resets internal counters.
 */
void cp_spinup_reset(void)
{
	spinup_process.counter     = SPINUP_MODSTEP_COUNTER_MAX;
	spinup_process.mod_counter = SPINUP_MODSTEP_COUNTER_MOD_MAX;
}

/**
 * Callback function to be hooked as handler for state
 * cps_spinup in control_process.c.
 * Implements a continuous increase of phase transition
 * rate, opposed to the 2-step implementation that
 * introduces a discontinuity that is known to lock the
 * motor.
 */
enum control_process_cb_state
control_process_spinup_cb(struct control_process * cps)
{
	return cps_cb_continue;
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

