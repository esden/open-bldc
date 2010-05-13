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

#include "types.h"
#include "comm_tim.h"
#include "pwm.h"

#include "control_process.h"

/* local definitions */
#define CONTROL_PROCESS_ALIGN_TIME 200
#define CONTROL_PROCESS_COARCE_MAX_SPINUP_STEP 30
#define CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV 5
#define CONTROL_PROCESS_SPINUP_DEC_DIV 500

enum control_process_states {
	cps_idle,
	cps_aligning,
	cps_coarce_spinup,
	cps_spinup,
	cps_spinning
};

/* local variables */
struct control_process {
	enum control_process_states state;
	int align_time;
	int coarce_spinup_time;
	int coarce_spinup_step;
	bool ignite;
	bool kill;
};

struct control_process control_process;

/* local functions */
void control_process_reset(void);

/* function implementations */
void control_process_init(void)
{
	control_process_reset();
}

void control_process_reset(void)
{
	control_process.state = cps_idle;
	control_process.align_time = CONTROL_PROCESS_ALIGN_TIME;
	control_process.coarce_spinup_time = 0;
	control_process.coarce_spinup_step = CONTROL_PROCESS_COARCE_MAX_SPINUP_STEP;
	control_process.ignite = false;
	control_process.kill = false;
}

void control_process_ignite(void)
{
	control_process_reset();
	comm_tim_reset();
	control_process.ignite = true;
}

void control_process_kill(void)
{
	pwm_off();
	comm_tim_trigger_comm = false;
	comm_tim_trigger_comm_once = false;
	control_process.kill = true;
	control_process.state = cps_idle;
}

void run_control_process(void)
{
	switch(control_process.state){
	case cps_idle:
		if(control_process.ignite){
			comm_tim_trigger_comm_once = true;
			control_process.ignite = false;
			control_process.state = cps_aligning;
		}
		break;
	case cps_aligning:
		if(control_process.align_time == 0){
			control_process.state = cps_coarce_spinup;
		}else{
			control_process.align_time--;
		}
		break;
	case cps_coarce_spinup:
		if(control_process.coarce_spinup_step > 0){
			if(control_process.coarce_spinup_time == 0){
				comm_tim_trigger_comm_once = true;

				control_process.coarce_spinup_time =
					control_process.coarce_spinup_step;

				if((control_process.coarce_spinup_step /
						CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV) == 0){
					control_process.coarce_spinup_step--;
				}else{
					control_process.coarce_spinup_step =
						control_process.coarce_spinup_step -
						(control_process.coarce_spinup_step /
							CONTROL_PROCESS_COARCE_SPINUP_DEC_DIV);
				}
			}else{
				control_process.coarce_spinup_time--;
			}
		}else{
			comm_tim_trigger_comm = true;
			control_process.state = cps_spinup;
		}
		break;
	case cps_spinup:
		comm_tim_data.freq =
			comm_tim_data.freq -
			(comm_tim_data.freq /
				CONTROL_PROCESS_SPINUP_DEC_DIV);
		if(comm_tim_data.freq < 27500){
			control_process.state = cps_spinning;
		}
		break;
	case cps_spinning:
		/* nothing yet */
		break;
	}
}
