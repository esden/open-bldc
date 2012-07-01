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

#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f1/gpio.h>

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "comm_tim.h"

#include "gprot.h"
#include "driver/led.h"
#include "driver/adc.h"
#include "driver/debug_pins.h"
#include "pwm/pwm.h"

/**
 * Commutation timer internal state
 */
struct comm_tim_state {
	volatile u32 update_count;              /**< update count since curr_time, needed for data.update_count calculation */
	volatile u16 next_prev_time;
};

struct comm_tim_data comm_tim_data;		/**< Commutation timer data instance */
static struct comm_tim_state comm_tim_state;    /**< Commutation timer internal state instance */
bool comm_tim_trigger_comm = false;		/**< Commutation timer trigger commutations flag */
bool comm_tim_trigger_comm_once = false;	/**< Commutation timer trigger one commutation flag */
bool comm_tim_trigger = false;			/**< Commutation timer trigger (it's an output not input) */

/**
 * Commutation timer hardware initialization.
 */
void comm_tim_init(void)
{

	comm_tim_data.freq = 65535;

	(void)gpc_setup_reg(GPROT_COMM_TIM_FREQ_REG_ADDR, &(comm_tim_data.freq));

	/* TIM2 clock enable */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);

	/* Enable the TIM2 gloabal interrupt. */
	nvic_enable_irq(NVIC_TIM2_IRQ);

	/* Reset TIM2 peripheral. */
	timer_reset(TIM2);

	/* TIM2 time base configuration */
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
		       TIM_CR1_CMS_EDGE,
		       TIM_CR1_DIR_UP);

	/* Set prescaler value */
	timer_set_prescaler(TIM2, 4);

	/* Disable preload. */
	timer_disable_preload(TIM2);

	/* Set continous mode. */
	timer_continuous_mode(TIM2);

	/* Set period to maximum */
	timer_set_period(TIM2, 65535);

	/* Disable outputs. */
	timer_disable_oc_output(TIM2, TIM_OC1);
	timer_disable_oc_output(TIM2, TIM_OC2);
	timer_disable_oc_output(TIM2, TIM_OC3);
	timer_disable_oc_output(TIM2, TIM_OC4);

	/* TIM2 Output Compare Timing Mode configuration: Channel1 */
	timer_disable_oc_clear(TIM2, TIM_OC1);
	timer_disable_oc_preload(TIM2, TIM_OC1);
	timer_set_oc_slow_mode(TIM2, TIM_OC1);
	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_FROZEN);
	//timer_set_oc_polarity_high(TIM2, TIM_OC1);

	/* Set initial capture compare value for OC1 */
	timer_set_oc_value(TIM2, TIM_OC1, comm_tim_data.freq);

	/* ARR reload enable */
	timer_disable_preload(TIM2);

	/* Counter enable */
	timer_enable_counter(TIM2);

	/* TIM2 Capture Compare 1 IT enable */
	timer_enable_irq(TIM2, TIM_DIER_CC1IE);
	/* TIM2 Update IT enable */
	timer_enable_irq(TIM2, TIM_DIER_UIE);

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
	u16 new_time = timer_get_counter(TIM2);
	comm_tim_data.prev_time = comm_tim_state.next_prev_time;
	comm_tim_data.curr_time = new_time;
	comm_tim_state.next_prev_time = new_time;
	comm_tim_data.update_count = comm_tim_state.update_count;
	comm_tim_state.update_count = 0;
}

/**
 * Update the commutation timer frequency using the value stored in
 * @ref comm_tim_data.freq
 */
void comm_tim_update_freq(void)
{
	timer_set_oc_value(TIM2, TIM_OC1,
			comm_tim_data.last_capture_time + comm_tim_data.freq);

}

/**
 * Update our last capture time
 *
 */
void comm_tim_update_capture(void)
{
	comm_tim_data.last_capture_time = timer_get_counter(TIM2);
	timer_set_oc_value(TIM2, TIM_OC1,
			   comm_tim_data.last_capture_time + comm_tim_data.freq);

	OFF(DP_EXT_SCL);
}

/**
 * Update the next prev time to the current time
 */
void comm_tim_update_next_prev(void)
{
	comm_tim_state.next_prev_time = timer_get_counter(TIM2);
}

/**
 * Update our last capture time and curr time
 *
 */
void comm_tim_update_capture_and_time(void)
{
	comm_tim_data.last_capture_time = timer_get_counter(TIM2);
	timer_set_oc_value(TIM2, TIM_OC1,
			comm_tim_data.last_capture_time + comm_tim_data.freq);

	comm_tim_data.prev_time = comm_tim_state.next_prev_time;
	comm_tim_data.curr_time = comm_tim_data.last_capture_time;
	comm_tim_state.next_prev_time = comm_tim_data.last_capture_time;
	comm_tim_data.update_count = comm_tim_state.update_count;
	comm_tim_state.update_count = 0;
}

/**
 * Timer 2 interrupt handler
 */
void tim2_isr(void)
{

	if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {
		timer_clear_flag(TIM2, TIM_SR_CC1IF);

		/* prepare for next comm */
		comm_tim_data.last_capture_time = timer_get_counter(TIM2);
		//comm_tim_data.last_capture_time = timer_get_ic_value(TIM2, TIM_OC1);
		//comm_tim_data.last_capture_time = TIM2_CCR1;

		/* triggering commutation event */
		if (comm_tim_trigger_comm || comm_tim_trigger_comm_once) {
			timer_generate_event(TIM1, TIM_EGR_COMG);
			//timer_generate_event(TIM1, TIM_EGR_COMG | TIM_EGR_UG);
		}

		/* (re)setting "semaphors" */
		comm_tim_trigger_comm_once = false;
		comm_tim_trigger = true;

		/* Set next comm time */
		timer_set_oc_value(TIM2, TIM_OC1,
				comm_tim_data.last_capture_time +
				(comm_tim_data.freq * 2));
		//timer_set_oc_value(TIM2, TIM_OC1,
		//		comm_tim_data.last_capture_time +
		//		65535);

		TOGGLE(DP_EXT_SCL);
	}

	if (timer_get_flag(TIM2, TIM_SR_UIF)) {
		timer_clear_flag(TIM2, TIM_SR_UIF);

		comm_tim_state.update_count++;
	}
}
