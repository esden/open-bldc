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

#ifndef __CONFIG_H
#define __CONFIG_H

/**
 * Enable and configure PWM phase reference signal generation.
 *
 * If set to value 1 - 6 or 1 - 12 depending on selected PWM scheme the
 * commutation subsystem will generate a signal on one of the LEDS that matches
 * the start of a selected PWM phase. To disable set it to -1.
 */
#define PWM_PHASE_TRIGGER 1

//#define ADC_COMM_TRIGGER 1

/**
 * Selection of PWM scheme.
 *
 * Set to one of the following:
 * - pwm_scheme_6step_h_pwm_l_on
 * - pwm_scheme_6step_h_on_l_pwm
 * - pwm_scheme_6step_on_pwm
 * - pwm_scheme_6step_pwm_on
 * - pwm_scheme_12step_pwm_on_pwm
 */
#define PWM_SCHEME pwm_scheme_6step_h_pwm_l_on

/**
 * PWM frequency
 */
#define PWM_FREQUENCY 24000

/*============================================================================
 * Debug pins configuration
 *============================================================================*/
#define DP_USE_EXT_I2C
#define DP_USE_ENCODER

/*============================================================================
 * CPU load process (idle process) configuration
 *============================================================================*/
/**
 * Time base of the CPU load calculation timeframe
 *
 * The time after which the counter should be taken and used for calculating
 * the cpu load in 10us
 */
#define CLP_TIME_BASE 10000

/**
 * IIR filter divider value applied to the cpu load calculation
 *
 * counter = ((counter * iir_val) + new_counter) / (iir_val + 1)
 */
#define CLP_IIR_VALUE 10

/**
 * Every how many time base times send the CPU load values to the PC
 */
#define CLP_REPORT_DIVIDER 10

/*============================================================================
 * Hardware based BEMF detection driver configuration
 *============================================================================*/

/**
 * Enable BEMF indication LED
 */
#define BEMF_HD_USE_LED

/**
 * Toggle the BEMF indication LED when getting an interrupt instead of
 * outputting it phase correct.
 */
#define BEMF_HD_LED_TOGGLE

/*============================================================================
 * USART driver configuration
 *============================================================================*/

/**
 * Baudrate to use
 */
#define USART_BAUD 230400

/*============================================================================
 * Control process configuration
 *============================================================================*/

/*----------------------------------------------------------------------------
 * Alignment configuration
 *----------------------------------------------------------------------------*/
/**
 * Enable motor alignment
 */
#define CP_ALIGN_ENABLE 1

/**
 * Default motor alignment time.
 */
#define CP_ALIGN_TIME 200

/**
 * Default motor alignement power.
 */
#define CP_ALIGN_POWER 300

/*----------------------------------------------------------------------------
 * Spinup two step configuration
 *----------------------------------------------------------------------------*/
/**
 * Default maximum delay between commutations while in coarse spinup state.
 */
#define CP_STS_COARSE_MAX_STEP 30

/**
 * Default decrement divider for coarse spinup.
 */
#define CP_STS_COARSE_DEC_DIV 50

/**
 * Default decrement divider for fine spinup.
 */
#define CP_STS_FINE_DEC_DIV 60000

/*----------------------------------------------------------------------------
 * Spinup soft timer configuration
 *----------------------------------------------------------------------------*/

/**
 * Default spinup power
 */
#define CP_SST_POWER 200

/**
 * Default maximum delay between commutations while spinning up
 */
#define CP_SST_MAX_STEP 1000

/**
 * Default minimum delay between commutations while spinning up
 *
 * When the minimum delay is reached the spinup process gives up.
 */
#define CP_SST_MIN_STEP 200

/**
 * Default decrement divider for spinup
 */
#define CP_SST_DEC_DIV 150

/**
 * How long to hold the maximum step if staying in open loop
 */
#define CP_SST_HOLD 0

/**
 * When is it safe to try to transition to closed loop.
 */
#define CP_SST_SAFE_FOR_CLOSED_LOOP 1000

#endif /* __CONFIG_H */
