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

#include <stm32/lib.h>

#include "config.h"
#include "pwm.h"

#include "pwm_utils.h"

void pwm_trigger(u16 zone){
    if(PWM_PHASE_TRIGGER == zone){
        GPIOC->BRR |= 0x00001000;
    }
    if(PWM_PHASE_TRIGGER == zone + 1){
        GPIOC->BSRR |= 0x00001000;
    }
}

void pwm_set_____hi(u16 phase){
    TIM_SelectOCxM(TIM1, phase, TIM_ForcedAction_Active );
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Enable);
    TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Enable);
}

void pwm_set_____lo(u16 phase){
    TIM_SelectOCxM(TIM1, phase, TIM_ForcedAction_InActive );
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Enable);
    TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Enable);
}

void pwm_set_pwm_hi(u16 phase){
    TIM_SelectOCxM(TIM1, phase, TIM_OCMode_PWM1);
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Enable);
    if(pwm_free_wheeling){
        TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Enable);
    }else{
        TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Disable);
    }
}

void pwm_set_pwm_lo(u16 phase){
    TIM_SelectOCxM(TIM1, phase, TIM_OCMode_PWM2);
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Enable);
    if(pwm_free_wheeling){
        TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Enable);
    }else{
        TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Disable);
    }
}

void pwm_set____off(u16 phase){
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Disable);
    TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Disable);
}
