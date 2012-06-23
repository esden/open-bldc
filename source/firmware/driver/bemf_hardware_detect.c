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

#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "driver/led.h"
#include "driver/debug_pins.h"
#include "comm_tim.h"

struct bemf_hd_data bemf_hd_data;
u16 bemf_line_state;

#ifdef BEMF_HD__USE_LED
#  ifdef BEMF_HD__LED_TOGGLE
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

#define BEMF_U_LINE GPIO10
#define BEMF_V_LINE GPIO11
#define BEMF_W_LINE GPIO12

/**
 * Initialize the hardware based BEMF detection peripherals
 */
void bemf_hd_init(void)
{

	bemf_hd_reset();

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN |
				                  RCC_APB2ENR_AFIOEN);

	nvic_enable_irq(NVIC_EXTI15_10_IRQ);

	/* GPIOB: EXTI Pin 10, 11, 12 as interrupt input
	 * Pin 10 -> BEMF/I_Sense of PHASE A
	 * Pin 11 -> BEMF/I_Sense of PHASE B
	 * Pin 12 -> BEMF/I_Sense of PHASE C
	 */
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_PULL_UPDOWN, GPIO10 | GPIO11 | GPIO12);
	gpio_set(GPIOB, GPIO10 | GPIO11 | GPIO12);

	exti_select_source(EXTI10, GPIOB);
	exti_set_trigger(EXTI10, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI10);

	exti_select_source(EXTI11, GPIOB);
	exti_set_trigger(EXTI11, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI11);

	exti_select_source(EXTI12, GPIOB);
	exti_set_trigger(EXTI12, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI12);
}

void bemf_hd_reset(void)
{
	bemf_hd_data.source = bemf_hd_phase_none;
	bemf_hd_data.trigger = false;
}

/**
 * External interrupt bank 15 to 10 handler (phase U V W)
 */
void exti15_10_isr(void)
{
	bemf_line_state = GPIOB_IDR;

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
	} else {
		comm_tim_update_next_prev();
	}

	exti_reset_request(EXTI0);

}
void exti1_isr(void)
{
	bemf_line_state = GPIOA_IDR;
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
	} else {
		comm_tim_update_next_prev();
	}

	exti_reset_request(EXTI1);
	ON(DP_EXT_SDA);

}

/**
 * External interrupt bank 2 handler (phase W)
 */
void exti2_isr(void)
{
	bemf_line_state = GPIOA_IDR;
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
	} else {
		comm_tim_update_next_prev();
	}

	exti_reset_request(EXTI2);
	ON(DP_EXT_SDA);

}
