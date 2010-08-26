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

/**
 * @file   comm_tim.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 02:05:17 2010
 *
 * @brief  Commutation timer hardware implementation.
 *
 * Implements the timer that is directly triggering the commutation event of
 * the PWM generation subsystem.
 *
 * @todo The comm timer should use a timer driver and not combine timer
 * peripheral handling code together with the commutation handling part.
 */

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "comm_tim.h"

#include "gprot.h"
#include "driver/led.h"
#include "driver/adc.h"
#include "pwm/pwm.h"

struct comm_tim_data comm_tim_data;		/**< Commutation timer data instance */
bool comm_tim_trigger_comm = false;		/**< Commutation timer trigger commutations flag */
bool comm_tim_trigger_comm_once = false;	/**< Commutation timer trigger one commutation flag */
bool comm_tim_trigger = false;			/**< Commutation timer trigger (it's an output not input) */

/**
 * Commutation timer hardware initialization.
 */
void comm_tim_init(void)
{
	NVIC_InitTypeDef nvic;
	TIM_TimeBaseInitTypeDef tim_base;
	TIM_OCInitTypeDef tim_oc;

	comm_tim_data.freq = 65535;

	gpc_setup_reg(GPROT_COMM_TIM_FREQ_REG_ADDR, &(comm_tim_data.freq));

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Enable the TIM2 gloabal interrupt */
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic);

	/* TIM2 time base configuration */
	tim_base.TIM_Period = 65535;
	tim_base.TIM_Prescaler = 0;
	tim_base.TIM_ClockDivision = 0;
	tim_base.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &tim_base);

	/* TIM2 prescaler configuration */
	TIM_PrescalerConfig(TIM2, 2, TIM_PSCReloadMode_Immediate);

	/* TIM2 Output Compare Timing Mode configuration: Channel1 */
	tim_oc.TIM_OCMode = TIM_OCMode_Timing;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = comm_tim_data.freq;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &tim_oc);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* TIM1 IT enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	TIM_Cmd(TIM2, ENABLE);

	comm_tim_reset();
}

/**
 * Reset the commutation timer internal state.
 */
void comm_tim_reset(void)
{
	comm_tim_trigger_comm = false;
	comm_tim_trigger_comm_once = false;
	comm_tim_data.freq = 65535;
}

/**
 * Record the time of the last commutation
 */
void comm_tim_capture_time(void)
{
	u16 new_time = TIM_GetCounter(TIM2);
	comm_tim_data.prev_time = comm_tim_data.curr_time;
	comm_tim_data.curr_time = new_time;
}

/**
 * Update the commutation timer frequency using the value stored in
 * @ref comm_tim_data.freq
 */
void comm_tim_update_freq(void)
{
	TIM_SetCompare1(TIM2,
			comm_tim_data.last_capture_time + comm_tim_data.freq);
}

/**
 * Timer 2 interrupt handler
 */
void tim2_irq_handler(void)
{

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		/* prepare for next comm */
		comm_tim_data.last_capture_time = TIM_GetCapture1(TIM2);

		/* triggering commutation event */
		if (comm_tim_trigger_comm || comm_tim_trigger_comm_once) {
			TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
			//TIM_GenerateEvent(TIM1, TIM_EventSource_COM | TIM_EventSource_Update);
		}

		/* (re)setting "semaphors" */
		comm_tim_trigger_comm_once = false;
		comm_tim_trigger = true;

		/* Set next comm time */
		TIM_SetCompare1(TIM2,
				comm_tim_data.last_capture_time +
				comm_tim_data.freq);
	}
}
