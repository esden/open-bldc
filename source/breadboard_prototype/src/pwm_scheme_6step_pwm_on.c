/*
 * Open-BLDC - Open BruschLess DC Motor Controller
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

/*
 * Table of the pwm scheme zone configurations:
 *
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
 */

#include <stm32/lib.h>

#include "pwm_utils.h"

#include "pwm_scheme_6step_pwm_on.h"

void pwm_scheme_6step_pwm_on(void){
    static int pwm_phase =1;

    switch(pwm_phase){
    case 1: // 000º
        pwm_trigger(1);

        /* Configure step 2 */
        pwm_set_____hi(PWM_PHASE_A);
        pwm_set_pwm_lo(PWM_PHASE_B);
        pwm_set____off(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 2: // 060º
        pwm_trigger(2);

        /* Configure step 3 */
        pwm_set____off(PWM_PHASE_A);
        pwm_set_____lo(PWM_PHASE_B);
        pwm_set_pwm_hi(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 3: // 120º
        pwm_trigger(3);

        /* Configure step 4 */
        pwm_set_pwm_lo(PWM_PHASE_A);
        pwm_set____off(PWM_PHASE_B);
        pwm_set_____hi(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 4: // 180º
        pwm_trigger(4);

        /* Configure step 4 */
        pwm_set_____lo(PWM_PHASE_A);
        pwm_set_pwm_hi(PWM_PHASE_B);
        pwm_set____off(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 5: // 220º
        pwm_trigger(5);

        /* Configure step 4 */
        pwm_set____off(PWM_PHASE_A);
        pwm_set_____hi(PWM_PHASE_B);
        pwm_set_pwm_lo(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 6: // 280º
        pwm_trigger(6);

        /* Configure step 4 */
        pwm_set_pwm_hi(PWM_PHASE_A);
        pwm_set____off(PWM_PHASE_B);
        pwm_set_____lo(PWM_PHASE_C);

        pwm_phase=1;
        break;
    }
}
