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

/*
 * Here you find the implementation of the Governor PC software protocol.
 */

#include <stm32/gpio.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "gprot.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"
#include "comm_tim.h"
#include "adc.h"

#define GPROT_FLAG_PWM_COMM (1 << 0)
#define GPROT_FLAG_COMM_TIM (1 << 1)
#define GPROT_FLAG_ADC_COMM (1 << 2)
#define GPROT_FLAG_ALL_LO (1 << 3)
#define GPROT_FLAG_ALL_HI (1 << 4)

u16 gprot_dummy_val;
u16 gprot_flag_reg;
u16 gprot_flag_reg_old;

/* Private function declarations */
void gprot_trigger_output(void *data);
void gprot_register_changed(void *data, u8 addr);
void gprot_update_flags(void);

/* Function implementations */
void gprot_init()
{
	gpc_init(gprot_trigger_output, 0, gprot_register_changed, 0);

	gprot_flag_reg = 0;
	gprot_flag_reg_old = 0;
	gpc_setup_reg(GPROT_FLAG_REG_ADDR, &gprot_flag_reg);

	gpc_setup_reg(GPROT_PWM_OFFSET_REG_ADDR, &pwm_offset);
	gpc_setup_reg(GPROT_PWM_VAL_REG_ADDR, &pwm_val);
	gpc_setup_reg(GPROT_COMM_TIM_FREQ_REG_ADDR, &comm_tim_freq);
	gpc_setup_reg(GPROT_ADC_LEVEL_RISING_REG_ADDR, &adc_level_rising);
	gpc_setup_reg(GPROT_ADC_LEVEL_FALLING_REG_ADDR, &adc_level_falling);
	gpc_setup_reg(GPROT_COMM_TIM_SPARK_ADVANCE_REG_ADDR, (u16 *)&comm_tim_spark_advance);
	gpc_setup_reg(GPROT_COMM_TIM_DIRECT_CUTOFF_REG_ADDR, &comm_tim_direct_cutoff);
	gpc_setup_reg(GPROT_COMM_TIM_IIR_POLE_REG_ADDR, &comm_tim_iir_pole);
}

void gprot_trigger_output(void *data)
{
	data = data;
	usart_enable_send();
}

void gprot_register_changed(void *data, u8 addr)
{
	data = data;
	if(addr == GPROT_FLAG_REG_ADDR){
		gprot_update_flags();
	}
}

void gprot_update_flags(void)
{
	if(gprot_flag_reg & GPROT_FLAG_PWM_COMM){
		pwm_comm();
		return;
	}

	if(gprot_flag_reg & GPROT_FLAG_COMM_TIM){
		comm_tim_on();
	}else{
		if(gprot_flag_reg_old & GPROT_FLAG_COMM_TIM){
			comm_tim_off();
		}
	}

	if(gprot_flag_reg & GPROT_FLAG_ADC_COMM){
		adc_comm = 1;
	}else{
		if(gprot_flag_reg_old & GPROT_FLAG_ADC_COMM){
			adc_comm = 0;
		}
	}

	if(gprot_flag_reg & GPROT_FLAG_ALL_LO){
		pwm_all_lo();
	}else{
		if(gprot_flag_reg_old & GPROT_FLAG_ALL_LO){
			pwm_off();
		}
	}

	if(gprot_flag_reg & GPROT_FLAG_ALL_HI){
		pwm_all_hi();
	}else{
		if(gprot_flag_reg_old & GPROT_FLAG_ALL_HI){
			pwm_off();
		}
	}

	/* add other flags here (up to 16) */

	gprot_flag_reg_old = gprot_flag_reg;
}
