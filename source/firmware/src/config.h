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

#ifndef __CONFIG_H
#define __CONFIG_H

/**
 * Default motor alignment time.
 */
#define CONTROL_PROCESS_ALIGN_TIME 200

/**
 * Default maximum delay between commutations while in coarse spinup state.
 */
#define CONTROL_PROCESS_COARSE_MAX_SPINUP_STEP 30

/**
 * Default decrement divider for coarse spinup.
 */
#define CONTROL_PROCESS_COARSE_SPINUP_DEC_DIV 50

/**
 * Default decrement divider for fine spinup.
 */
#define CONTROL_PROCESS_SPINUP_DEC_DIV 60000

/**
 * Enable and configure PWM phase reference signal generation.
 *
 * If set to value 1 - 6 or 1 - 12 depending on selected PWM scheme the
 * commutation subsystem will generate a signal on one of the LEDS that matches
 * the start of a selected PWM phase. To disable set it to -1.
 */
#define PWM_PHASE_TRIGGER 1

//#define ADC_COMM_TRIGGER 1

/**
 * Selection of PWM scheme.
 *
 * Set to one of the following:
 * - pwm_scheme_6step_h_pwm_l_on
 * - pwm_scheme_6step_h_on_l_pwm
 * - pwm_scheme_6step_on_pwm
 * - pwm_scheme_6step_pwm_on
 * - pwm_scheme_12step_pwm_on_pwm
 */
#define PWM_SCHEME pwm_scheme_6step_h_pwm_l_on

#endif /* __CONFIG_H */
