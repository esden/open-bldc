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
 *  | 1| 2| 3| 4| 5| 6|
 * -+--+--+--+--+--+--+
 * A|p+|p+|  |--|--|  |
 * -+--+--+--+--+--+--+
 * B|  |--|--|  |p+|p+|
 * -+--+--+--+--+--+--+
 * C|--|  |p+|p+|  |--|
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

#include <stm32/tim.h>
#include <stm32/adc.h>

#include "types.h"
#include "pwm_utils.h"
#include "adc.h"
#include "comm_process.h"

#include "pwm_scheme_6step_h_pwm_l_on.h"

inline void pwm_scheme_6step_h_pwm_l_on(void){
    static int pwm_phase = 1;

    switch(pwm_phase){
    case 1: // 000º

        adc_set(ADC_CHANNEL_B);
	comm_process_config_and_reset(COMM_PROCESS_FALLING);

        /* Configure step 2 */
	pwm_set_a_hpwm_b_low__c_off();

        pwm_phase++;
        break;
    case 2: // 060º

        adc_set(ADC_CHANNEL_C);
	comm_process_config(COMM_PROCESS_RISING);

        /* Configure step 3 */
	pwm_set_a_off__b_low__c_hpwm();

        pwm_phase++;
        break;
    case 3: // 120º

        adc_set(ADC_CHANNEL_A);
	comm_process_config_and_reset(COMM_PROCESS_FALLING);

        /* Configure step 4 */
	pwm_set_a_low__b_off__c_hpwm();

        pwm_phase++;
        break;
    case 4: // 180º

        adc_set(ADC_CHANNEL_B);
	comm_process_config_and_reset(COMM_PROCESS_RISING);

        /* Configure step 4 */
	pwm_set_a_low__b_hpwm_c_off();

        pwm_phase++;
        break;
    case 5: // 220º

        adc_set(ADC_CHANNEL_C);
	comm_process_config_and_reset(COMM_PROCESS_FALLING);

        /* Configure step 4 */
	pwm_set_a_off__b_hpwm_c_low();

        pwm_phase++;
        break;
    case 6: // 280º

        adc_set(ADC_CHANNEL_A);
	comm_process_config_and_reset(COMM_PROCESS_RISING);

        /* Configure step 4 */
	pwm_set_a_hpwm_b_off__c_low();

        pwm_phase=1;
        break;
    }
}
