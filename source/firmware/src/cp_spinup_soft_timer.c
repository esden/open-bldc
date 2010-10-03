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
#include "params_guarded.h"

#include "cp_spinup.h"
#include "control_process.h"
#include "cp_spinning.h"

#include "types.h"
#include "pwm/pwm.h"
#include "driver/led.h"
#include "driver/sys_tick.h"
#include "driver/debug_pins.h"
#include "comm_tim.h"

/**
 * Control process spinup trigger variable
 */
static bool cps_trigger;

#define CP__SST_FIXED_POINT 16

/**
 * Trigger source for spinup state.
 */
static bool *control_process_spinup_trigger = &cps_trigger;

/**
 * Internal process variables for spinup callback.
 */
struct spinup_process {
	int timer;  /**< Soft timer id used for time reference */
	u32 step;   /**< Current coarce spinup step */
	u32 hold;   /**< How long to hold the maximum comm time (for testing) */
};
static struct spinup_process spinup_process;

/*============================================================================
 * Private functions forward declarations
 *============================================================================*/
static enum control_process_cb_state
control_process_spinup_cb(/*@unused@*/ struct control_process *cps);

static enum control_process_cb_state
control_process_spinup_state_in_cb(/*@unused@*/ struct control_process *cps);

static enum control_process_cb_state
control_process_spinup_state_out_cb(/*@unused@*/ struct control_process *cps);

static void control_process_soft_timer_callback(int id);

/*============================================================================
 * Function implementations
 *============================================================================*/

/**
 * Initialization of the spinup callback process, currently
 * redirecting to @ref cp_spinup_reset.
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
 * Sets spin up state to spinup_state_coarse and resets
 * internal countdowns.
 */
void cp_spinup_reset(void)
{
	spinup_process.step = (u32)(CP__SST_MAX_STEP << CP__SST_FIXED_POINT);
	spinup_process.hold = (u32)(CP__SST_HOLD);
	PWM_SET(CP__SST_POWER);
}

/**
 * Callback function to be hooked as handler for state
 * cps_spinup in control_process.c.
 *
 * Implements a simplicistic spin up using a soft timer as a time base.
 */
enum control_process_cb_state
control_process_spinup_cb(struct control_process *cps)
{

	/* Check if the spinning process is able to run */
	if (cp_spinning_ready() &&
		(spinup_process.step < (CP__SST_SAFE_FOR_CLOSED_LOOP << CP__SST_FIXED_POINT))) {
		cps->state = cps_spinning;
		return cps_cb_resume_control;
	}

	/* Calculate the duration of the next commutation cycle */
	spinup_process.step = spinup_process.step -
	    (spinup_process.step / CP__SST_DEC_DIV);

	/* Give up if the commutation speed exeeds limit and the spinning
	 * process is still not happy
	 */
	if ((spinup_process.step >> CP__SST_FIXED_POINT) <= CP__SST_MIN_STEP) {
		spinup_process.step = (u32)(CP__SST_MIN_STEP << CP__SST_FIXED_POINT);
		if(spinup_process.hold > 0) {
			spinup_process.hold--;
			return cps_cb_continue;
		}else{
			return cps_cb_exit_control;
		}
	}

	return cps_cb_continue;
}

/**
 * Callback function called before entering control
 * process state cps_spinup.
 */
enum control_process_cb_state
control_process_spinup_state_in_cb(struct control_process *cps)
{
	cps = cps;

	OFF(LED_GREEN);
	cp_spinup_reset();

	spinup_process.timer =
	    sys_tick_timer_register(control_process_soft_timer_callback,
				    spinup_process.step >> CP__SST_FIXED_POINT);

	return cps_cb_continue;
}

/**
 * Callback function called after leaving control
 * process state cps_spinup, before entering the
 * next control process state.
 */
enum control_process_cb_state
control_process_spinup_state_out_cb(struct control_process *cps)
{
	cps = cps;

	sys_tick_timer_unregister(spinup_process.timer);

	return cps_cb_continue;
}

/**
 * Time reference software timer callback function.
 *
 * This callback generates the trigger for the spinup control process, it also
 * commutates the motor and updates to the next timestep value.
 */
void control_process_soft_timer_callback(int id)
{
	/* update the capture time of the comm timer so it stays in sync with us */
	//comm_tim_update_capture();

	/* commutate the motor because it is about time */
	pwm_comm();

	/* trigger the control process spinup process */
	cps_trigger = true;

	TOGGLE(DP_ENC_A);

	/* set new time for us */
	sys_tick_timer_update(id, spinup_process.step >> CP__SST_FIXED_POINT);
}
