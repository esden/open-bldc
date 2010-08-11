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

#include "types.h"
#include "32bit_tim.h"

#include "led.h"

volatile u32 tim_freq = 1030;
u32 tim_was_updated = 0;
u32 tim_last_upd = 0;

void tim_init(void)
{
	NVIC_InitTypeDef nvic;
	TIM_TimeBaseInitTypeDef tim_base;
	TIM_OCInitTypeDef tim_oc;

	/* TIM2 and TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |
			       RCC_APB1Periph_TIM3, ENABLE);

	/*
	 * TIM2 is representing the lsb part of the timer.
	 * TIM3 is representing the msb part of the timer.
	 *
	 * The lsb timer is advancing the msb timer every time it wraps
	 * (update event).
	 */

	/* TIM2 (LSB) config ------------------------------------------ */

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

	/* TIM2 Output Compare Timing Mode configuration: Channel1 */
	tim_oc.TIM_OCMode = TIM_OCMode_Timing;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = tim_freq & 0xFFFF;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &tim_oc);

	/* TIM2 disable output compare preload */
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* TIM2 Update master output trigger selection */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* TIM3 (MSB) config ------------------------------------------ */

	/* Enable the TIM3 global inturrupt */
	nvic.NVIC_IRQChannel = TIM3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic);

	/* TIM3 time base configuration */
	tim_base.TIM_Period = 65535;
	tim_base.TIM_Prescaler = 0;
	tim_base.TIM_ClockDivision = 0;
	tim_base.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &tim_base);

	/* TIM3 Output Compare Timing Mode configuration: Channel1 */
	tim_oc.TIM_OCMode = TIM_OCMode_Timing;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = tim_freq >> 16;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &tim_oc);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	/* TIM3 slave mode selection: trigger mode */
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);

	/* TIM3 input trigger selection */
	TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);

	/* Switch on/off interrupt handlers --------------------------- */
	if (tim_freq < 0x10000) {
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	} else {
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
	}

	/* Enable timers */
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

}

void tim_update(void)
{
	u16 curr_time_lsb = TIM_GetCounter(TIM2);
	u16 curr_time_msb = TIM_GetCounter(TIM3);
	u16 next_capture_msb;
	u16 next_capture_lsb;

	u32 curr_time = curr_time_msb;
	curr_time <<= 16;
	curr_time |= curr_time_lsb;
	u32 next_capture;

	if (!tim_was_updated) {
		tim_last_upd = curr_time;
		tim_was_updated = 1;
		return;
	}

	tim_freq = (curr_time - tim_last_upd);
	next_capture = tim_freq / 2;

	tim_last_upd = curr_time;

	next_capture_lsb = next_capture & 0xFFFF;
	next_capture_msb = next_capture >> 16;

	TIM_SetCompare1(TIM2, curr_time_lsb + next_capture_lsb);
	TIM_SetCompare1(TIM3, curr_time_msb + next_capture_msb);
	if (next_capture_msb == 0) {
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	} else {
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	}
}

void tim2_irq_handler(void)
{
	u16 last_capture_lsb = TIM_GetCapture1(TIM2);
	u16 last_capture_msb = TIM_GetCapture1(TIM3);
	u16 tim_freq_lsb;
	u16 tim_freq_msb;

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		tim_freq_lsb = tim_freq & 0xFFFF;
		tim_freq_msb = tim_freq >> 16;
		TIM_SetCompare1(TIM2, last_capture_lsb + tim_freq_lsb);
		TIM_SetCompare1(TIM3, last_capture_msb + tim_freq_msb);
		if (tim_freq_msb == 0) {
			TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
			TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
		} else {
			TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
			TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
		}
		LED_ORANGE_TOGGLE();
	}
}

void tim3_irq_handler(void)
{

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
		LED_RED_TOGGLE();
	}
}
