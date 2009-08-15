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

/*
 * Table of the pwm scheme zone configurations:
 *
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
 */

#include <stm32/tim.h>

#include "pwm.h"
#include "pwm_utils.h"

#include "pwm_scheme_12step_pwm_on_pwm.h"

void pwm_scheme_12step_pwm_on_pwm(void){
    static int pwm_phase =1;

    switch(pwm_phase){
    case 1: // 000º
        pwm_trigger(1);

        /* Configure step 2 */
        pwm_set_____hi(PWM_PHASE_A);
        pwm_set____off(PWM_PHASE_B);
        pwm_set_pwm_lo(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 2: // 030º
        pwm_trigger(2);

        /* Configure step 3 */
        pwm_set_____hi(PWM_PHASE_A);
        pwm_set_pwm_lo(PWM_PHASE_B);
        pwm_set____off(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 3: // 060º
        pwm_trigger(3);

        /* Configure step 4 */
        pwm_set_pwm_hi(PWM_PHASE_A);
        pwm_set_____lo(PWM_PHASE_B);
        pwm_set____off(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 4: // 090º
        pwm_trigger(4);

        /* Configure step 5 */
        pwm_set____off(PWM_PHASE_A);
        pwm_set_____lo(PWM_PHASE_B);
        pwm_set_pwm_hi(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 5: // 120º
        pwm_trigger(5);

        /* Configure step 6 */
        pwm_set____off(PWM_PHASE_A);
        pwm_set_pwm_lo(PWM_PHASE_B);
        pwm_set_____hi(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 6: // 150º
        pwm_trigger(6);

        /* Configure step 7 */
        pwm_set_pwm_lo(PWM_PHASE_A);
        pwm_set____off(PWM_PHASE_B);
        pwm_set_____hi(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 7: // 180º
        pwm_trigger(7);

        /* Configure step 8 */
        pwm_set_____lo(PWM_PHASE_A);
        pwm_set____off(PWM_PHASE_B);
        pwm_set_pwm_hi(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 8: // 210º
        pwm_trigger(8);

        /* Configure step 9 */
        pwm_set_____lo(PWM_PHASE_A);
        pwm_set_pwm_hi(PWM_PHASE_B);
        pwm_set____off(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 9: // 240º
        pwm_trigger(9);

        /* Configure step 10 */
        pwm_set_pwm_lo(PWM_PHASE_A);
        pwm_set_____hi(PWM_PHASE_B);
        pwm_set____off(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 10: // 270º
        pwm_trigger(10);

        /* Configure step 11 */
        pwm_set____off(PWM_PHASE_A);
        pwm_set_____hi(PWM_PHASE_B);
        pwm_set_pwm_lo(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 11: // 300º
        pwm_trigger(11);

        /* Configure step 12 */
        pwm_set____off(PWM_PHASE_A);
        pwm_set_pwm_hi(PWM_PHASE_B);
        pwm_set_____lo(PWM_PHASE_C);

        pwm_phase++;
        break;
    case 12: // 330º
        pwm_trigger(12);

        /* Configure step 1 */
        pwm_set_pwm_hi(PWM_PHASE_A);
        pwm_set____off(PWM_PHASE_B);
        pwm_set_____lo(PWM_PHASE_C);

        pwm_phase=1;
        break;
    }
}
