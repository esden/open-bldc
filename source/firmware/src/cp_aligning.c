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

#include "cp_aligning.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"

/**
 * Trigger source for aligning state.
 */
bool *control_process_aligning_trigger = &comm_tim_trigger;

struct aligning_process {
	int align_time;			   /**< Allignement timer */
};
static struct aligning_process aligning_process;

enum control_process_cb_state
control_process_aligning_cb(struct control_process * cps) {
	if (aligning_process.align_time == 0) {
		cps->state = cps_spinup;
	} else {
		aligning_process.align_time--;
	}
	return cps_cb_continue;
}

void cp_aligning_reset(void) {
	aligning_process.align_time = CONTROL_PROCESS_ALIGN_TIME;
}
