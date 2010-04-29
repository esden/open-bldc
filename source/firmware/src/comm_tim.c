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

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "comm_tim.h"

#include "gprot.h"
#include "led.h"
#include "adc.h"
#include "pwm/pwm.h"

volatile u16 comm_tim_freq = 65535;
uint16_t comm_tim_capture = 0;
volatile u32 comm_tim_last_time = 0;
volatile s16 comm_tim_spark_advance = 0;
volatile u16 comm_tim_direct_cutoff = 8000;
volatile u16 comm_tim_iir_pole = 15;

void comm_tim_init(void)
{
	NVIC_InitTypeDef nvic;
	TIM_TimeBaseInitTypeDef tim_base;
	TIM_OCInitTypeDef       tim_oc;

	comm_tim_capture = 0;
	comm_tim_spark_advance = 0;
	comm_tim_direct_cutoff = 8000;
	comm_tim_iir_pole = 15;

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
	tim_oc.TIM_Pulse = comm_tim_freq;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &tim_oc);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* TIM1 IT enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	comm_tim_off();
}

void comm_tim_on(void)
{
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

void comm_tim_off(void)
{
	/* TIM2 disable counter */
	TIM_Cmd(TIM2, DISABLE);

	pwm_all_lo();
}

void comm_tim_rising_set_next_comm()
{
	if((adc_comm_data.last_value == 0) ||
		(adc_comm_data.last_value > adc_comm_data.curr_value)){
		comm_tim_freq -= 10;
	}else{
		u32 curr_time = TIM_GetCounter(TIM2);
		u32 pwm_time = curr_time - comm_tim_last_time;
		u32 adc_rise = adc_comm_data.curr_value - adc_comm_data.last_value;
		u32 adc_zero = adc_comm_data.zero_value - adc_comm_data.last_value;
		u32 match_time = (adc_zero * pwm_time) / adc_rise;
		u32 new_freq = ((comm_tim_last_time - comm_tim_capture) + match_time) * 2;
		u32 comm_tim_freq_cpy = comm_tim_freq;
		u32 comm_tim_iir_pole_cpy = comm_tim_iir_pole;

		if(new_freq < (comm_tim_freq - comm_tim_direct_cutoff)){
			comm_tim_freq -= 10;
			LED_BLUE_ON();
		}else if(new_freq > (comm_tim_freq + comm_tim_direct_cutoff)){
			comm_tim_freq += 10;
			LED_BLUE_ON();
		}else{
			comm_tim_freq = ((comm_tim_freq_cpy * comm_tim_iir_pole_cpy) + new_freq)
				/ (comm_tim_iir_pole_cpy + 1);
			LED_BLUE_OFF();
		}

		TIM_SetCompare1(TIM2, comm_tim_capture + comm_tim_freq + comm_tim_spark_advance);
	}
}

void comm_tim_falling_set_next_comm()
{
	if((adc_comm_data.last_value == 0) ||
		(adc_comm_data.last_value < adc_comm_data.curr_value)){
		comm_tim_freq -= 10;
	}else{
		u32 curr_time = TIM_GetCounter(TIM2);
		u32 pwm_time = curr_time - comm_tim_last_time;
		u32 adc_rise = adc_comm_data.last_value - adc_comm_data.curr_value;
		u32 adc_zero = adc_comm_data.last_value - adc_comm_data.zero_value;
		u32 match_time = (adc_zero * pwm_time) / adc_rise;
		u32 new_freq = ((comm_tim_last_time - comm_tim_capture) + match_time) * 2;
		u32 comm_tim_freq_cpy = comm_tim_freq;
		u32 comm_tim_iir_pole_cpy = comm_tim_iir_pole;

		if(new_freq < (comm_tim_freq - comm_tim_direct_cutoff)){
			comm_tim_freq -= 10;
			LED_BLUE_ON();
		}else if(new_freq > (comm_tim_freq + comm_tim_direct_cutoff)){
			comm_tim_freq += 10;
			LED_BLUE_ON();
		}else{
			comm_tim_freq = ((comm_tim_freq_cpy * comm_tim_iir_pole_cpy) + new_freq)
				/ (comm_tim_iir_pole_cpy + 1);
			LED_BLUE_OFF();
		}

		TIM_SetCompare1(TIM2, comm_tim_capture + comm_tim_freq + comm_tim_spark_advance);
	}
}

void comm_tim_set_next_comm(void)
{

	if(!adc_comm_data.crossed){
		comm_tim_last_time = TIM_GetCounter(TIM2);
	}else if(adc_comm){
		if(adc_comm_data.rising){
			comm_tim_rising_set_next_comm();
		}else{
			comm_tim_falling_set_next_comm();
		}
	}
}

void tim2_irq_handler(void)
{

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		/* Toggling ORANGE LED and triggering commutation with frequency = 73.24 Hz */
		//LED_ORANGE_TOGGLE();
		TIM_GenerateEvent(TIM1, TIM_EventSource_COM);//  | TIM_EventSource_Update);

		/* Preparing next comm time */
		comm_tim_capture = TIM_GetCapture1(TIM2);

		TIM_SetCompare1(TIM2, comm_tim_capture + comm_tim_freq);
	}
}
