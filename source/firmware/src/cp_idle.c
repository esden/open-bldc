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
 * @file   cp_idle.c
 * @author Tobias Fuchs <twh.fuchs@gmail.com>
 * @date   Thu Aug 19 15:59:42 2010
 *
 * @brief  Controller process idle strategy
 *
 * Default control process implementation for idle state.
 */

#include "cp_idle.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"

/**
 * Trigger source for the idle state.
 */
bool *control_process_idle_trigger = &comm_tim_trigger;

enum control_process_cb_state
control_process_idle_cb(struct control_process * cps) {
	if (cps->ignite) {
		comm_tim_trigger_comm_once = true;
		cps->ignite = false;
		cps->state  = cps_aligning;
	}
	return cps_cb_continue;
}

void cp_idle_reset(void) {
}
