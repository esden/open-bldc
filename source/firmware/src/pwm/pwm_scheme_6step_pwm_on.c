/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2009 by Piotr Esden-Tempski <piotr@esden.net>
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
 * @file   pwm_scheme_6step_pwm_on.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:53:26 2010
 *
 * @brief  Implementation of 6step PWM ON scheme
 *
 * Table of the pwm scheme zone configurations when driving:
 * @verbatim
 *  | 1| 2| 3| 4| 5| 6|
 * -+--+--+--+--+--+--+
 * A|p+|++|  |p-|--|  |
 * -+--+--+--+--+--+--+
 * B|  |p-|--|  |p+|++|
 * -+--+--+--+--+--+--+
 * C|--|  |p+|++|  |p-|
 * -+--+--+--+--+--+--+
 *  |  |  |  |  |  |  '- 360º
 *  |  |  |  |  |  '---- 300º
 *  |  |  |  |  '------- 240º
 *  |  |  |  '---------- 180º
 *  |  |  '------------- 120º
 *  |  '----------------  60º
 *  '-------------------   0º
 *
 * Legend:
 * p+: PWM on the high side
 * p-: PWM on the low side
 * --: Low side on
 * ++: High side on
 *   : Floating/NC
 * @endverbatim
 */

#include <stm32/tim.h>

#include "pwm_utils.h"

#include "pwm_scheme_6step_pwm_on.h"

/**
 * PWM scheme configuration function
 */
void pwm_scheme_6step_pwm_on(void)
{
	static int pwm_phase = 1;

	switch (pwm_phase) {
	case 1:		// 000º

                /* Configure step 2 */
		pwm_set_a_high_b_lpwm_c_off();

		pwm_phase++;
		break;
	case 2:		// 060º

		/* Configure step 3 */
		pwm_set_a_off__b_low__c_hpwm();

		pwm_phase++;
		break;
	case 3:		// 120º

		/* Configure step 4 */
		pwm_set_a_lpwm_b_off__c_high();

		pwm_phase++;
		break;
	case 4:		// 180º

		/* Configure step 5 */
		pwm_set_a_low__b_hpwm_c_off();

		pwm_phase++;
		break;
	case 5:		// 220º

		/* Configure step 6 */
		pwm_set_a_off__b_high_c_lpwm();

		pwm_phase++;
		break;
	case 6:		// 280º

		/* Configure step 1 */
		pwm_set_a_hpwm_b_off__c_low();

		pwm_phase = 1;
		break;
	}
}
