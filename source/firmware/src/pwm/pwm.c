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

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f1/gpio.h>

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
/** Default PWM mode */
#define PWM__MODE PWM_DRIVE
/** Default PWM duty cycle value */
#define PWM__VALUE 300
/** Default PWM offset for ADC triggering */
#define PWM__OFFSET 187

//volatile uint16_t pwm_val = 700;
//volatile uint16_t pwm_offset = 250;
/** Current PWM direction */
volatile enum pwm_mode pwm_mode = PWM__MODE;
/** Current PWM duty cycle */
volatile uint32_t pwm_val = PWM__VALUE;
/** Current PWM offset for ADC triggering */
static volatile uint16_t pwm_offset = PWM__OFFSET;

/**
 * Initialize the three phase (6outputs) PWM peripheral and internal state.
 */
void pwm_init(void)
{

	(void)gpc_setup_reg(GPROT_PWM_OFFSET_REG_ADDR, &pwm_offset);

	/* Enable clock for TIM1 subsystem */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_TIM1EN |
		RCC_APB2ENR_IOPAEN |
		RCC_APB2ENR_IOPBEN |
		RCC_APB2ENR_AFIOEN);

	/* GPIOA: TIM1 channel 1, 2 and 3 as alternate function
	   push-pull */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_TIM1_CH1 |
		      GPIO_TIM1_CH2 |
		      GPIO_TIM1_CH3);

	/* GPIOB: TIM1 channel 1N, 2N and 3N as alternate function
	 * push-pull
	 */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_TIM1_CH1N |
		      GPIO_TIM1_CH2N |
		      GPIO_TIM1_CH3N);

	/* Enable TIM1 commutation interrupt */
	nvic_enable_irq(NVIC_TIM1_TRG_COM_IRQ);

	/* Enable TIM1 capture/compare interrupt */
	nvic_enable_irq(NVIC_TIM1_CC_IRQ);

	/* Reset TIM1 peripheral */
	timer_reset(TIM1);

	/* Timer global mode:
	 * - No divider
	 * - alignment edge
	 * - direction up
	 */
	timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT,
		       TIM_CR1_CMS_EDGE,
		       TIM_CR1_DIR_UP);

	/* Enable preload. */
	timer_enable_preload(TIM1);

	/* Continous mode. */
	timer_continuous_mode(TIM1);

	/* Period (PWM__FREQUENCY) */
	timer_set_period(TIM1, PWM__BASE_CLOCK / PWM__FREQUENCY);

	/* Configure break and deadtime */
	timer_set_deadtime(TIM1, 10);
	timer_set_enabled_off_state_in_idle_mode(TIM1);
	timer_set_enabled_off_state_in_run_mode(TIM1);
	timer_disable_break(TIM1);
	timer_set_break_polarity_high(TIM1);
	timer_disable_break_automatic_output(TIM1);
	timer_set_break_lock(TIM1, TIM_BDTR_LOCK_OFF);

	/* -- OC1 and OC1N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC1);
	timer_disable_oc_output(TIM1, TIM_OC1N);

	/* Configure global mode of line 1. */
	timer_disable_oc_clear(TIM1, TIM_OC1);
	timer_enable_oc_preload(TIM1, TIM_OC1);
	timer_set_oc_slow_mode(TIM1, TIM_OC1);
	timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);

	/* Configure OC1. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1);
	timer_set_oc_idle_state_set(TIM1, TIM_OC1);

	/* Configure OC1N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC1N);

	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC1, pwm_val);

	/* -- OC2 and OC2N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC2);
	timer_disable_oc_output(TIM1, TIM_OC2N);

	/* Configure global mode of line 2. */
	timer_disable_oc_clear(TIM1, TIM_OC2);
	timer_enable_oc_preload(TIM1, TIM_OC2);
	timer_set_oc_slow_mode(TIM1, TIM_OC2);
	timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);

	/* Configure OC2. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2);
	timer_set_oc_idle_state_set(TIM1, TIM_OC2);

	/* Configure OC2N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC2N);

	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC2, pwm_val);

	/* -- OC3 and OC3N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC3);
	timer_disable_oc_output(TIM1, TIM_OC3N);

	/* Configure global mode of line 3. */
	timer_disable_oc_clear(TIM1, TIM_OC3);
	timer_enable_oc_preload(TIM1, TIM_OC3);
	timer_set_oc_slow_mode(TIM1, TIM_OC3);
	timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

	/* Configure OC3. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3);
	timer_set_oc_idle_state_set(TIM1, TIM_OC3);

	/* Configure OC3N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC3N);

	/* Set the capture compare value for OC3. */
	timer_set_oc_value(TIM1, TIM_OC3, pwm_val);

	/* ---- */
	/* ARR reload enable */
	timer_enable_preload(TIM1);

	/* Enable preload of complementary channel configurations and update on COM event */
	timer_enable_preload_complementry_enable_bits(TIM1);

	/* Enable outputs in the break subsystem */
	timer_enable_break_main_output(TIM1);

	/* Counter enable */
	timer_enable_counter(TIM1);

	/* Enable commutation interrupt */
	timer_enable_irq(TIM1, TIM_DIER_COMIE);
	//timer_enable_irq(TIM1, TIM_DIER_COMIE | TIM_DIER_CC4IE);

	/* Setting default state of pwm to pwm_off */
	pwm_off();
}

/**
 * Trigger one commutation event.
 */
void pwm_comm(void)
{
	timer_generate_event(TIM1, TIM_EGR_COMG);
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
void tim1_trg_com_isr(void)
{
	timer_clear_flag(TIM1, TIM_SR_COMIF);

	ON(LED_BLUE);

	timer_set_oc_value(TIM1, TIM_OC1, pwm_val);
	timer_set_oc_value(TIM1, TIM_OC2, pwm_val);
	timer_set_oc_value(TIM1, TIM_OC3, pwm_val);

	PWM__SCHEME();
	OFF(LED_BLUE);
}

/**
 * PWM timer capture compare event interrupt handler
 */
#if 0
void tim1_cc_isr(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) {
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);

		/* Toggling ORANGE LED ca. 22us after pwm duty cycle start of PWM1 */
		//if(pwm_trig_led) TOGGLE(LED_ORANGE);
	}
}
#endif
