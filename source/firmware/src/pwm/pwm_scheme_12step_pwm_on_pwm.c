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
 * @file   pwm_scheme_12step_pwm_on_pwm.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:57:17 2010
 *
 * @brief  Implementation of the 12step PWM ON PWM scheme.
 *
 * Table of the pwm scheme zone configurations:
 * @verbatim
 *  | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|
 * -+--+--+--+--+--+--+--+--+--+--+--+--+
 * A|p+|++|++|p+|  |  |p-|--|--|p-|  |  |
 * -+--+--+--+--+--+--+--+--+--+--+--+--+
 * B|  |  |p-|--|--|p-|  |  |p+|++|++|p+|
 * -+--+--+--+--+--+--+--+--+--+--+--+--+
 * C|--|p-|  |  |p+|++|++|p+|  |  |p-|--|
 * -+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |  |  |  |  |  |  |  |  |  |  |  |  '- 360º
 *  |  |  |  |  |  |  |  |  |  |  |  '---- 330º
 *  |  |  |  |  |  |  |  |  |  |  '------- 300º
 *  |  |  |  |  |  |  |  |  |  '---------- 270º
 *  |  |  |  |  |  |  |  |  '------------- 240º
 *  |  |  |  |  |  |  |  '---------------- 210º
 *  |  |  |  |  |  |  '------------------- 180º
 *  |  |  |  |  |  '---------------------- 150º
 *  |  |  |  |  '------------------------- 120º
 *  |  |  |  '----------------------------  90º
 *  |  |  '-------------------------------  60º
 *  |  '----------------------------------  30º
 *  '-------------------------------------   0º
 *
 * Legend:
 * p+: PWM on the high side
 * p-: PWM on the low side
 * --: Low side on
 * ++: High side on
 *   : Floating/NC
 * \endverbatim
 */

#include <stm32/tim.h>

#include "pwm.h"
#include "pwm_utils.h"

#include "pwm_scheme_12step_pwm_on_pwm.h"

/**
 * PWM scheme configuration function
 */
void pwm_scheme_12step_pwm_on_pwm(void)
{
	static int pwm_phase = 1;

	switch (pwm_phase) {
	case 1:		// 000º

		/* Configure step 2 */
		pwm_set_a_high_b_off__c_lpwm();

		pwm_phase++;
		break;
	case 2:		// 030º

		/* Configure step 3 */
		pwm_set_a_high_b_lpwm_c_off();

		pwm_phase++;
		break;
	case 3:		// 060º

		/* Configure step 4 */
		pwm_set_a_hpwm_b_low__c_off();

		pwm_phase++;
		break;
	case 4:		// 090º

		/* Configure step 5 */
		pwm_set_a_off__b_low__c_hpwm();

		pwm_phase++;
		break;
	case 5:		// 120º

		/* Configure step 6 */
		pwm_set_a_off__b_lpwm_c_high();

		pwm_phase++;
		break;
	case 6:		// 150º

		/* Configure step 7 */
		pwm_set_a_lpwm_b_off__c_high();

		pwm_phase++;
		break;
	case 7:		// 180º

		/* Configure step 8 */
		pwm_set_a_low__b_off__c_hpwm();

		pwm_phase++;
		break;
	case 8:		// 210º

		/* Configure step 9 */
		pwm_set_a_low__b_hpwm_c_off();

		pwm_phase++;
		break;
	case 9:		// 240º

		/* Configure step 10 */
		pwm_set_a_lpwm_b_high_c_off();

		pwm_phase++;
		break;
	case 10:		// 270º

		/* Configure step 11 */
		pwm_set_a_off__b_high_c_lpwm();

		pwm_phase++;
		break;
	case 11:		// 300º

		/* Configure step 12 */
		pwm_set_a_off__b_hpwm_c_low();

		pwm_phase++;
		break;
	case 12:		// 330º

		/* Configure step 1 */
		pwm_set_a_hpwm_b_off__c_low();

		pwm_phase = 1;
		break;
	}
}
