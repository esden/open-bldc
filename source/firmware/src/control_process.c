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
 * @file   control_process.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:27:42 2010
 *
 * @brief  User space control process implementation.
 *
 * This process is being called periodically from the main while loop and
 * handles the high level control of the system. The main state machine of the
 * whole controller is implemented here.
 */

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

#include "control_process.h"

/**
 * Default motor alignment time.
 *
 * @todo move to central configuration header
 */
#define CONTROL_PROCESS_ALIGN_TIME 200

/**
 * Default maximum delay between commutations while in coarce spinup state.
 *
 * @todo move to central configuration header
 */
#define CONTROL_PROCESS_COARCE_MAX_SPINUP_STEP 30

/**
 * Default decrement divider for coarce spinup.
 *
 * @todo move to central configuration header
 */
#define CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV 50

/**
 * Default decrement divider for fine spinup.
 *
 * @todo move to central configuration header
 */
#define CONTROL_PROCESS_SPINUP_DEC_DIV 60000

/**
 * Control process state machine states
 */
enum control_process_states {
	cps_idle,
	cps_aligning,
	cps_coarce_spinup,
	cps_spinup,
	cps_spinning
};

/**
 * Internal state structure
 */
struct control_process {
	enum control_process_states state; /**< State machine state */
	int align_time;			   /**< Allignement timer */
	int coarce_spinup_time;		   /**< Coarce spinup timer */
	int coarce_spinup_step;		   /**< Current coarce spinup step */
	bool ignite;			   /**< Ignite mode trigger */
	bool kill;			   /**< Kill motor trigger */
	u32 bemf_crossing_counter;	   /**< Valid BEMF crossing detection counter */
	u32 bemf_lost_crossing_counter;    /**< Invalid BEMF crossing detection counter */
};

struct control_process control_process; /**< Internal state struct instance */

/* local function forward declarations */
void control_process_reset(void);

/* function implementations */
/**
 * Initialize internal state of the control process
 */
void control_process_init(void)
{
	control_process_reset();
}

/**
 * Reset the internal state of the control process
 */
void control_process_reset(void)
{
	control_process.state = cps_idle;
	control_process.align_time = CONTROL_PROCESS_ALIGN_TIME;
	control_process.coarce_spinup_time = 0;
	control_process.coarce_spinup_step =
	    CONTROL_PROCESS_COARCE_MAX_SPINUP_STEP;
	control_process.ignite = false;
	control_process.kill = false;
	control_process.bemf_crossing_counter = 0;
	control_process.bemf_lost_crossing_counter = 0;
}

/**
 * Trigger motor ignition
 */
void control_process_ignite(void)
{
	control_process_reset();
	comm_tim_reset();
	control_process.ignite = true;
}

/**
 * Immediately kill the motor
 *
 * Calling this function will immediately stop sending any signals to the power
 * stage. Can be used for example when a fault is being detected.
 */
void control_process_kill(void)
{
	pwm_off();
	comm_tim_trigger_comm = false;
	comm_tim_trigger_comm_once = false;
	control_process.kill = true;
	control_process.state = cps_idle;
	comm_process_closed_loop_off();
}

/**
 * Main periodic control process body.
 *
 * It implements the main state machine of the motor controller.
 */
void run_control_process(void)
{
	switch (control_process.state) {
	case cps_idle:
		if (control_process.ignite) {
			comm_tim_trigger_comm_once = true;
			control_process.ignite = false;
			control_process.state = cps_aligning;
		}
		break;
	case cps_aligning:
		if (control_process.align_time == 0) {
			control_process.state = cps_coarce_spinup;
		} else {
			control_process.align_time--;
		}
		break;
	case cps_coarce_spinup:
		if (control_process.coarce_spinup_step > 0) {
			if (control_process.coarce_spinup_time == 0) {
				comm_tim_trigger_comm_once = true;

				control_process.coarce_spinup_time =
				    control_process.coarce_spinup_step;

				if ((control_process.coarce_spinup_step /
				     CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV) ==
				    0) {
					control_process.coarce_spinup_step--;
				} else {
					control_process.coarce_spinup_step =
					    control_process.coarce_spinup_step -
					    (control_process.coarce_spinup_step
					     /
					     CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV);
				}
			} else {
				control_process.coarce_spinup_time--;
			}
		} else {
			comm_tim_trigger_comm = true;
			control_process.state = cps_spinup;
		}
		break;
	case cps_spinup:
		if (comm_data.bemf_crossing_detected) {
			comm_data.bemf_crossing_detected = false;
			control_process.bemf_crossing_counter++;
			control_process.bemf_lost_crossing_counter = 0;
		} else {
			control_process.bemf_crossing_counter = 0;
			control_process.bemf_lost_crossing_counter++;
		}

		if ((control_process.bemf_crossing_counter > 2) &&
		    (comm_tim_data.freq < 30000) &&
		    (comm_data.in_range_counter > 2)) {
			comm_process_closed_loop_on();
			control_process.state = cps_spinning;
			LED_RED_ON();
			return;
		}

		comm_tim_data.freq =
		    comm_tim_data.freq -
		    (comm_tim_data.freq / CONTROL_PROCESS_SPINUP_DEC_DIV);
		if (comm_tim_data.freq < 10000) {
			control_process_kill();
		}
		break;
	case cps_spinning:
		if (comm_data.bemf_crossing_detected) {
			comm_data.bemf_crossing_detected = false;
			control_process.bemf_crossing_counter++;
			control_process.bemf_lost_crossing_counter = 0;
			LED_RED_ON();
		} else {
			control_process.bemf_crossing_counter = 0;
			control_process.bemf_lost_crossing_counter++;
			LED_RED_OFF();
		}

		if (control_process.bemf_lost_crossing_counter > 10) {
			comm_process_closed_loop_off();
			control_process_kill();
		}

		break;
	}
}
