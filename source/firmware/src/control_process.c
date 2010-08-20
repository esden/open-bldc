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

#include "config.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"
#include "spinup.h"

#include "control_process.h"

/* local function forward declarations */
void control_process_reset(void);

/* Note: control_process_spinup_cb is defined in external 
   spinup strategy that implements spinup.h. 
*/
enum control_process_cb_state control_process_idle_cb(struct control_process * cps);
enum control_process_cb_state control_process_aligning_cb(struct control_process * cps);
enum control_process_cb_state control_process_spinning_cb(struct control_process * cps);
enum control_process_cb_state control_process_error_cb(struct control_process * cps);

struct control_process control_process; /**< Internal state struct instance */

typedef enum control_process_cb_state (*cps_callback)(struct control_process * cps);
static cps_callback control_process_cb_register[cbs_num_states];

/* function implementations */

/** Register a callback function for given control 
 *  process state. 
 *  Example from control_process_init(): 
 *
 * 		control_process_register_cb(cps_idle, control_process_idle_cb);
 *
 */
void control_process_register_cb(enum control_process_cb_state cp_state, 
																 enum control_process_cb_state (*callback_fun)(struct control_process * cps))
{ 
	control_process_cb_register[cp_state] = callback_fun; 
}

/**
 * Initialize internal state of the control process
 */
void control_process_init(void)
{
	control_process_reset();
	spinup_reset(); 

	control_process_register_cb(cps_idle,     control_process_idle_cb);
	control_process_register_cb(cps_aligning, control_process_aligning_cb);
	control_process_register_cb(cps_spinup,   control_process_spinup_cb);
	control_process_register_cb(cps_spinning, control_process_spinning_cb);
	control_process_register_cb(cps_error,    control_process_error_cb);
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
	control_process.kill   = false;
	control_process.bemf_crossing_counter      = 0;
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
	comm_tim_trigger_comm      = false;
	comm_tim_trigger_comm_once = false;
	control_process.kill       = true;
	control_process.state      = cps_idle;
	comm_process_closed_loop_off();
}

enum control_process_cb_state
control_process_idle_cb(struct control_process * cps) { 
	if (cps->ignite) {
		comm_tim_trigger_comm_once = true;
		cps->ignite = false;
		cps->state  = cps_aligning;
	}
	return cps_cb_continue;
}

enum control_process_cb_state
control_process_aligning_cb(struct control_process * cps) { 
	if (cps->align_time == 0) {
		cps->state = cps_spinup;
	} else {
		cps->align_time--;
	}
	return cps_cb_continue;
}


enum control_process_cb_state
control_process_spinning_cb(struct control_process * cps) { 
	if (comm_data.bemf_crossing_detected) {
		comm_data.bemf_crossing_detected = false;
		cps->bemf_crossing_counter++;
		cps->bemf_lost_crossing_counter = 0;
		LED_RED_ON();
	} else {
		cps->bemf_crossing_counter = 0;
		cps->bemf_lost_crossing_counter++;
		LED_RED_OFF();
	}

	if (cps->bemf_lost_crossing_counter > 10) {
		comm_process_closed_loop_off();
		control_process_kill();
	}
	return cps_cb_continue;
}

enum control_process_cb_state
control_process_error_cb(struct control_process * cps) { 
	// TODO: Error handling for undefined 
	// control process state here. 
	return cps_cb_continue;
}

/**
 * Main periodic control process body.
 *
 * It implements the main state machine of the motor controller.
 */
void run_control_process(void)
{
	enum control_process_cb_state cb_ret; 
	cb_ret = control_process_cb_register[control_process.state](&control_process); 
	
	if(cb_ret < 0 && cb_ret >= cbs_num_states) { 
		control_process_cb_register[cps_error](&control_process);
	}
}


