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
 * @file   cp_aligning.c
 * @author Tobias Fuchs <twh.fuchs@gmail.com>
 * @date   Thu Aug 19 15:59:42 2010
 *
 * @brief  Control process aligning strategy
 *
 * Default control process implementation for aligning state.
 */

#include "config.h"

#include "cp_aligning.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

/**
 * Trigger source for aligning state.
 */
bool *control_process_aligning_trigger = &comm_tim_trigger;

/**
 * Internal process variables for aligning callback.
 */
struct aligning_process {
	int align_time;			   /**< Allignement timer */
};
static struct aligning_process aligning_process;

/**
 * Callback function to be hooked as handler for state
 * cps_aligning in control_process.c.
 * Implements a simple countdown from @ref CP_ALIGN_TIME
 * to 0.
 * Finally sets transition to control process state spinup by
 * setting control_process.state to cps_spinup.
 */
enum control_process_cb_state
control_process_aligning_cb(struct control_process *cps)
{
#if CP_ALIGN_ENABLE == 1
	if (aligning_process.align_time == 0) {
		cps->state = cps_spinup;
		LED_RED_TOGGLE();
	} else {
		aligning_process.align_time--;
	}
#else
	cps->state = cps_spinup;
#endif /* CP_ALIGN_ENABLE == 1 */
	return cps_cb_continue;
}

/**
 * Initialization of the aligning callback process.
 * Calls cp_aligning_reset and registers control_process_aligning_cb
 * as handler for control process state cps_aligning.
 */
void cp_aligning_init(void)
{
	cp_aligning_reset();
	control_process_register_cb(cps_aligning,
				    control_process_aligning_trigger,
				    control_process_aligning_cb, 0, 0);
}

/**
 * Reset function for the aligning callback process.
 * Sets countdown to state transition back to
 * @ref CP_ALIGN_TIME.
 */
void cp_aligning_reset(void)
{
	aligning_process.align_time = CP_ALIGN_TIME;
}

/**
 * Callback function called before entering control process state
 * cps_aligning
 */
enum control_process_cb_state
control_process_aligning_state_in_cb(struct control_process * cps) {
#if CP_ALIGN_ENABLE == 1
	pwm_comm();
#endif

	return cps_cb_continue;
}
