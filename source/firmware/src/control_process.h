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

#ifndef __CONTROL_PROCESS_H
#define __CONTROL_PROCESS_H

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
enum control_process_state {
	cps_idle=0,
	cps_aligning,
	cps_spinup,
	cps_spinning, 
	cps_error, 
	cbs_num_states, 
};

enum control_process_cb_state { 
	cps_cb_error=0, 
	cps_cb_continue, 
	cps_cb_break, 
	cps_cb_finished, 
	cbs_cb_num_states
};

/**
 * Internal state structure
 */
struct control_process {
	enum control_process_state state; /**< State machine state */
	int align_time;			   /**< Allignement timer */
	int coarce_spinup_time;		   /**< Coarce spinup timer */
	int coarce_spinup_step;		   /**< Current coarce spinup step */
	bool ignite;			   /**< Ignite mode trigger */
	bool kill;			   /**< Kill motor trigger */
	u32 bemf_crossing_counter;	   /**< Valid BEMF crossing detection counter */
	u32 bemf_lost_crossing_counter;    /**< Invalid BEMF crossing detection counter */
};


void control_process_init(void);
void control_process_ignite(void);
void control_process_kill(void);
void run_control_process(void);
void control_process_register_cb(enum control_process_cb_state cp_state, 
																 enum control_process_cb_state (*callback_fun)(struct control_process * cps));

#endif /* __CONTROL_PROCESS_H */
