/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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
 * @file   bemf_hardware_detect.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:37:44 2010
 *
 * @brief  Hardware based commutation detection implementation.
 *
 * This implementation uses a comparator output as source of zero crossing
 * detection.
 */

#include "config.h"

#include "types.h"

#include "bemf_hardware_detect.h"

#include <stm32/misc.h>
#include <stm32/exti.h>
#include <stm32/rcc.h>
#include <stm32/gpio.h>

#include "driver/led.h"
#include "driver/debug_pins.h"
#include "comm_tim.h"

struct bemf_hd_data bemf_hd_data;
u16 bemf_line_state;

#ifdef BEMF_HD_USE_LED
#  ifdef BEMF_HD_LED_TOGGLE
#    define BEMF_HD_LED_RISING() TOGGLE(LED_ORANGE)
#    define BEMF_HD_LED_FALLING() TOGGLE(LED_ORANGE)
#  else
#    define BEMF_HD_LED_RISING() OFF(LED_ORANGE)
#    define BEMF_HD_LED_FALLING() ON(LED_ORANGE)
#  endif
#else
#    define BEMF_HD_LED_RISING() ((void)0)
#    define BEMF_HD_LED_FALLING() ((void)0)
#endif

/**
 * Initialize the hardware based BEMF detection peripherals
 */
void bemf_hd_init(void)
{
	NVIC_InitTypeDef nvic;
	EXTI_InitTypeDef exti;
	GPIO_InitTypeDef gpio;

	bemf_hd_reset();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			       RCC_APB2Periph_AFIO, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	nvic.NVIC_IRQChannel = EXTI0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&nvic);

	/* GPIOA: EXTI Pin 0, 1, 2 as interrupt input
	 * Pin 0 -> BEMF/I_Sense of PHASE A
	 * Pin 1 -> BEMF/I_Sense of PHASE B
	 * Pin 2 -> BEMF/I_Sense of PHASE C
	 */
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	exti.EXTI_Line = EXTI_Line0;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	exti.EXTI_Line = EXTI_Line1;
	EXTI_Init(&exti);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	exti.EXTI_Line = EXTI_Line2;
	EXTI_Init(&exti);

}

void bemf_hd_reset(void)
{
	bemf_hd_data.source = bemf_hd_phase_none;
	bemf_hd_data.trigger = false;
}

/**
 * External interrupt bank 0 handler (phase U)
 */
void exti0_irq_handler(void)
{
	bemf_line_state = GPIOA->IDR;
	OFF(DP_EXT_SDA);

	/*
	 * Take care of oscillating interrupt triggers and filter them out
	 * also detect the source of the interrupt.
	 */
	if ((bemf_hd_data.source != bemf_hd_phase_u_rising) &&
		(bemf_hd_data.source != bemf_hd_phase_u_falling)) {
		comm_tim_update_capture_and_time();
		if ((bemf_line_state & (1 << 0)) != 0) {
			bemf_hd_data.source = bemf_hd_phase_u_rising;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_RISING();
		}else{
			bemf_hd_data.source = bemf_hd_phase_u_falling;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_FALLING();
		}
	}

	EXTI_ClearITPendingBit(EXTI_Line0);
	ON(DP_EXT_SDA);

}

/**
 * External interrupt bank 1 handler (phase V)
 */
void exti1_irq_handler(void)
{
	bemf_line_state = GPIOA->IDR;
	OFF(DP_EXT_SDA);

	/*
	 * Take care of oscillating interrupt triggers and filter them out
	 * also detect the source of the interrupt.
	 */
	if ((bemf_hd_data.source != bemf_hd_phase_v_rising) &&
		(bemf_hd_data.source != bemf_hd_phase_v_falling)) {
		comm_tim_update_capture_and_time();
		if ((bemf_line_state & (1 << 1)) != 0) {
			bemf_hd_data.source = bemf_hd_phase_v_rising;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_RISING();
		}else{
			bemf_hd_data.source = bemf_hd_phase_v_falling;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_FALLING();
		}
	}

	EXTI_ClearITPendingBit(EXTI_Line1);
	ON(DP_EXT_SDA);

}

/**
 * External interrupt bank 2 handler (phase W)
 */
void exti2_irq_handler(void)
{
	bemf_line_state = GPIOA->IDR;
	OFF(DP_EXT_SDA);

	/*
	 * Take care of oscillating interrupt triggers and filter them out
	 * also detect the source of the interrupt.
	 */
	if ((bemf_hd_data.source != bemf_hd_phase_w_rising) &&
		(bemf_hd_data.source != bemf_hd_phase_w_falling)) {
		comm_tim_update_capture_and_time();
		if ((bemf_line_state & (1 << 2)) != 0) {
			bemf_hd_data.source = bemf_hd_phase_w_rising;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_RISING();
		}else{
			bemf_hd_data.source = bemf_hd_phase_w_falling;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_FALLING();
		}
	}

	EXTI_ClearITPendingBit(EXTI_Line2);
	ON(DP_EXT_SDA);

}
