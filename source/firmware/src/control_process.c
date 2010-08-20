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
#include "config.h"

#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

#include "cp_spinup.h"
#include "cp_idle.h"
#include "cp_aligning.h"
#include "cp_spinning.h"
#include "cp_error.h"

#include "control_process.h"

/* local function forward declarations */
void control_process_reset(void);

/* Note: control_process_<state>_cb is defined in external
 * <state> strategy that implements <state>.h.
 */

struct control_process control_process; /**< Internal state struct instance */

/**
 * Control process state callback function pointer definition.
 */
typedef enum control_process_cb_state (*cps_callback)(struct control_process * cps);

/**
 * Control process event hook slot definition.
 */
struct cps_event_hook {
	bool *trigger;         /**< The trigger the callback should be called upon */
	cps_callback callback; /**< Pointer to the callback function */
};

static struct cps_event_hook control_process_cb_hook_register[cps_num_states];

/* function implementations */

/**
 * Register a callback function for given control process state.
 *
 * These are:
 * (see @ref control_process.h -> #control_process_state)
 *
 * - cps_error
 * - cps_idle
 * - cpd_aligning
 * - cps_spinup
 * - cps_spinning
 *
 * The number of existing process states (currently 5) is stored in
 * cps_num_states.
 *
 * In order to register a callback for a specific process state, use
 * @code
 * control_process_register_cb(<state>, <callback fun>);
 * @endcode
 *
 * Example from @ref control_process_init():
 * @code
 * control_process_register_cb(cps_idle, control_process_idle_trigger, control_process_idle_cb);
 * @endcode
 */
void control_process_register_cb(enum control_process_state cp_state,
				 bool *trigger,
				 enum control_process_cb_state (*callback_fun)(struct control_process * cps))
{
	control_process_cb_hook_register[cp_state].trigger = trigger;
	control_process_cb_hook_register[cp_state].callback = callback_fun;
}

/**
 * Initialize internal state of the control process
 * Also calls callback init functions for every
 * process state.
 */
void control_process_init(void)
{
	control_process_reset();

	cp_idle_init();
	cp_spinup_init();
	cp_aligning_init();
	cp_spinning_init();
	cp_error_init();

	control_process_register_cb(cps_idle,
				    control_process_idle_trigger,
				    control_process_idle_cb);
	control_process_register_cb(cps_aligning,
				    control_process_aligning_trigger,
				    control_process_aligning_cb);
	control_process_register_cb(cps_spinup,
				    control_process_spinup_trigger,
				    control_process_spinup_cb);
	control_process_register_cb(cps_spinning,
				    control_process_spinning_trigger,
				    control_process_spinning_cb);
	control_process_register_cb(cps_error,
				    control_process_error_trigger,
				    control_process_error_cb);
}

/**
 * Reset the internal state of the control process.
 * Also calls callback reset functions for every
 * process state.
 */
void control_process_reset(void)
{
	cp_idle_reset();
	cp_spinup_reset();
	cp_aligning_reset();
	cp_spinning_reset();
	cp_error_reset();

	control_process.state  = cps_idle;
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

/**
 * Main periodic control process body.
 *
 * It implements the main state machine of the motor controller.
 *
 * Calls registered callback functions according to the current
 * state of the control process.
 * If a callback function returns an invalid code, the callback
 * function for state cps_error is called.
 *
 * Exits the closed loop and kills the control process if a
 * callback function returns cps_cb_exit_control.
 */
void run_control_process(void)
{
	enum control_process_cb_state cb_ret = cps_error;

	if (*control_process_cb_hook_register[control_process.state].trigger) {
		*control_process_cb_hook_register[control_process.state].trigger = false;
		cb_ret = control_process_cb_hook_register[control_process.state].callback(&control_process);
	}

	if (cb_ret < 0 || cb_ret >= cps_num_states) {
		control_process_cb_hook_register[cps_error].callback(&control_process);
	}
	// Could also be implemented in error handling strategy:
	if (cb_ret == cps_cb_exit_control) {
		comm_process_closed_loop_off();
		control_process_kill();
	}
}


