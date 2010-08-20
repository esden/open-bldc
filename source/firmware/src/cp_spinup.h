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

#ifndef __CP_SPINUP_H
#define __CP_SPINUP_H

#include "control_process.h"

extern bool *control_process_spinup_trigger;

void cp_spinup_init(void);
void cp_spinup_reset(void);

enum control_process_cb_state
control_process_spinup_state_in_cb(struct control_process * cps);

enum control_process_cb_state
control_process_spinup_cb(struct control_process * cps);

enum control_process_cb_state
control_process_spinup_state_out_cb(struct control_process * cps);

#endif /* __CP_SPINUP_H */
