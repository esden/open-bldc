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

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>
#include <stm32/gpio.h>

#include "config.h"
#include "pwm_schemes.h"
#include "pwm_utils.h"

#include "pwm.h"

#include "led.h"

#define PWM_VALUE 130;

volatile uint16_t pwm_val = 130;

void pwm_init(void){
    NVIC_InitTypeDef nvic;
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef tim_base;
    TIM_OCInitTypeDef       tim_oc;
    TIM_BDTRInitTypeDef     tim_bdtr;


    /* Enable clock for TIM1 subsystem */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,
                           ENABLE);

    /* Enable TIM1 interrupt */
    nvic.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    /* Enable TIM1 interrupt */
    nvic.NVIC_IRQChannel = TIM1_CC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    /* GPIOA: TIM1 channel 1, 2 and 3 as alternate function
       push-pull */
    gpio.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    /* GPIOB: TIM1 channel 1N, 2N and 3N as alternate function
     * push-pull
     */
    gpio.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &gpio);

    /* Time base configuration */
    tim_base.TIM_Period = 1999;
    tim_base.TIM_Prescaler = 0;
    tim_base.TIM_ClockDivision = 0;
    tim_base.TIM_CounterMode = TIM_CounterMode_Up;
    tim_base.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &tim_base);

    /* TIM1 channel 1, 2 and 3 settings */
    tim_oc.TIM_OCMode       = TIM_OCMode_Timing;
    tim_oc.TIM_OutputState  = TIM_OutputState_Enable;
    tim_oc.TIM_OutputNState = TIM_OutputNState_Enable;
    tim_oc.TIM_Pulse        = PWM_VALUE;
    tim_oc.TIM_OCPolarity   = TIM_OCPolarity_High;
    tim_oc.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    tim_oc.TIM_OCIdleState  = TIM_OCIdleState_Set;
    tim_oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;

    TIM_OC1Init(TIM1, &tim_oc);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM1, &tim_oc);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM1, &tim_oc);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* TIM1 configure channel 4 as adc trigger source */
    tim_oc.TIM_OCMode       = TIM_OCMode_PWM2;
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM1, &tim_oc);

    /* Automatic Output enable, break, dead time and lock configuration */
    tim_bdtr.TIM_OSSRState       = TIM_OSSRState_Enable;
    tim_bdtr.TIM_OSSIState       = TIM_OSSIState_Enable;
    tim_bdtr.TIM_LOCKLevel       = TIM_LOCKLevel_OFF;
    tim_bdtr.TIM_DeadTime        = 10;
    tim_bdtr.TIM_Break           = TIM_Break_Disable;
    tim_bdtr.TIM_BreakPolarity   = TIM_BreakPolarity_High;
    tim_bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

    TIM_BDTRConfig(TIM1, &tim_bdtr);

    TIM_CCPreloadControl(TIM1, ENABLE);

    /* Enable COM and CC interrupt */
    TIM_ITConfig(TIM1, TIM_IT_COM | TIM_IT_CC4, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* Main output enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void pwm_comm(void){
    TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
}

void pwm_off(void){
    pwm_set_pwm_hi(PWM_PHASE_A);
    pwm_set_____lo(PWM_PHASE_B);
    pwm_set____off(PWM_PHASE_C);
}

void tim1_trg_com_irq_handler(void){
    TIM_ClearITPendingBit(TIM1, TIM_IT_COM);

    //LED_ORANGE_TOGGLE();

    TIM_SetCompare1(TIM1, pwm_val);
    TIM_SetCompare2(TIM1, pwm_val);
    TIM_SetCompare3(TIM1, pwm_val);
    TIM_SetCompare4(TIM1, pwm_val);

    PWM_SCHEME();
}

void tim1_cc_irq_handler(void){
    if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET){
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);

        /* Toggling ORANGE LED ca. 22us after pwm duty cycle start of PWM1 */
        LED_ORANGE_TOGGLE();
    }
}
