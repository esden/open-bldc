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

#ifndef __COMM_PROCESS_H
#define __COMM_PROCESS_H

#define COMM_PROCESS_FALLING FALSE
#define COMM_PROCESS_RISING TRUE

struct comm_params {
	s16 spark_advance;
	u16 direct_cutoff;
	u16 direct_cutoff_slope;
	u16 iir;
	u16 hold_off;
};

struct comm_data {
	bool bemf_crossing_detected;
	u16 calculated_freq;
	u32 in_range_counter;
};

extern struct comm_params comm_params;
extern struct comm_data comm_data;
extern s32 new_cycle_time;

void comm_process_init(void);
void comm_process_reset(void);
void comm_process_config(bool rising);
void comm_process_config_and_reset(bool rising);
void comm_process_closed_loop_on(void);
void comm_process_closed_loop_off(void);
void run_comm_process(void);

#endif /* __COMM_PROCESS_H */
