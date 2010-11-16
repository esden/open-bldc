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
 * @file   pwm.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:57:56 2010
 *
 * @brief  PWM subsystem global implementation
 *
 * This is the umbrella above all the PWM schemes.
 */

#include "config.h"

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>
#include <stm32/gpio.h>

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "gprot.h"
#include "pwm_schemes.h"
#include "pwm_utils.h"

#include "pwm/pwm.h"

#include "driver/led.h"

//#define PWM__VALUE 700
//#define PWM__OFFSET 250
/** Default PWM direction */
#define PWM__DIR PWM_FORWARD
/** Default PWM duty cycle value */
#define PWM__VALUE 300
/** Default PWM offset for ADC triggering */
#define PWM__OFFSET 187

//volatile uint16_t pwm_val = 700;
//volatile uint16_t pwm_offset = 250;
/** Current PWM direction */
volatile enum pwm_dir pwm_dir = PWM__DIR;
/** Current PWM duty cycle */
volatile uint32_t pwm_val = PWM__VALUE;
/** Current PWM offset for ADC triggering */
static volatile uint16_t pwm_offset = PWM__OFFSET;

/**
 * Initialize the three phase (6outputs) PWM peripheral and internal state.
 */
void pwm_init(void)
{
	NVIC_InitTypeDef nvic;
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim_base;
	TIM_OCInitTypeDef tim_oc;
	TIM_BDTRInitTypeDef tim_bdtr;

	(void)gpc_setup_reg(GPROT_PWM_OFFSET_REG_ADDR, &pwm_offset);

	/* Enable clock for TIM1 subsystem */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |
			       RCC_APB2Periph_GPIOA |
			       RCC_APB2Periph_GPIOB, ENABLE);

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
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	/* GPIOB: TIM1 channel 1N, 2N and 3N as alternate function
	 * push-pull
	 */
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &gpio);

	/* Time base configuration */
	tim_base.TIM_Period = PWM__BASE_CLOCK / PWM__FREQUENCY;
	tim_base.TIM_Prescaler = 0;
	tim_base.TIM_ClockDivision = 0;
	tim_base.TIM_CounterMode = TIM_CounterMode_Up;
	tim_base.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &tim_base);

	/* TIM1 channel 1, 2 and 3 settings */
	tim_oc.TIM_OCMode = TIM_OCMode_Timing;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_OutputNState = TIM_OutputNState_Enable;
	tim_oc.TIM_Pulse = pwm_val;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc.TIM_OCNPolarity = TIM_OCNPolarity_High;
	tim_oc.TIM_OCIdleState = TIM_OCIdleState_Set;
	tim_oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;

	TIM_OC1Init(TIM1, &tim_oc);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM1, &tim_oc);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1, &tim_oc);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* TIM1 configure channel 4 as adc trigger source */
	tim_oc.TIM_OCMode = TIM_OCMode_PWM2;
	tim_oc.TIM_Pulse = pwm_offset;
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM1, &tim_oc);

	/* Automatic Output enable, break, dead time and lock configuration */
	tim_bdtr.TIM_OSSRState = TIM_OSSRState_Enable;
	tim_bdtr.TIM_OSSIState = TIM_OSSIState_Enable;
	tim_bdtr.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	tim_bdtr.TIM_DeadTime = 10;
	tim_bdtr.TIM_Break = TIM_Break_Disable;
	tim_bdtr.TIM_BreakPolarity = TIM_BreakPolarity_High;
	tim_bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

	TIM_BDTRConfig(TIM1, &tim_bdtr);

	TIM_CCPreloadControl(TIM1, ENABLE);

	/* Enable COM and CC interrupt */
	TIM_ITConfig(TIM1, TIM_IT_COM, ENABLE);
	//TIM_ITConfig(TIM1, TIM_IT_COM | TIM_IT_CC4, ENABLE);

	/* TIM1 enable counter */
	TIM_Cmd(TIM1, ENABLE);

	/* Main output enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	/* Setting default state of pwm to pwm_off */
	pwm_off();
}

/**
 * Trigger one commutation event.
 */
void pwm_comm(void)
{
	TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
}

/**
 * Switch off all outputs
 */
void pwm_off(void)
{
	pwm_set_all_off();
	pwm_comm();
}

/**
 * Switch on the low side only
 */
void pwm_all_lo(void)
{
	pwm_set_all_low();
	pwm_comm();
}

/**
 * Switch on high side only
 */
void pwm_all_hi(void)
{
	pwm_set_all_high();
	pwm_comm();
}

/**
 * PWM timer commutation event interrupt handler
 */
void tim1_trg_com_irq_handler(void)
{
	TIM_ClearITPendingBit(TIM1, TIM_IT_COM);

	ON(LED_BLUE);

	TIM_SetCompare1(TIM1, pwm_val);
	TIM_SetCompare2(TIM1, pwm_val);
	TIM_SetCompare3(TIM1, pwm_val);
	TIM_SetCompare4(TIM1, pwm_offset);

	PWM__SCHEME();
	OFF(LED_BLUE);
}

/**
 * PWM timer capture compare event interrupt handler
 */
void tim1_cc_irq_handler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) {
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);

		/* Toggling ORANGE LED ca. 22us after pwm duty cycle start of PWM1 */
		//if(pwm_trig_led) TOGGLE(LED_ORANGE);
	}
}
