/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2009-2010 by Piotr Esden-Tempski <piotr@esden.net>
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

#ifndef __COMM_TIM_H
#define __COMM_TIM_H

struct comm_tim_data {
	volatile u16 last_capture_time;
	volatile u16 curr_time;
	volatile u16 prev_time;
	volatile u16 freq;
};

extern struct comm_tim_data comm_tim_data;

void comm_tim_init(void);
void comm_tim_on(void);
void comm_tim_off(void);
void comm_tim_capture_time(void);
void comm_tim_update_freq(void);

#endif /* __COMM_TIM_H */
