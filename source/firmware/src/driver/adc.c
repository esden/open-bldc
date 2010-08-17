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
 * @file   adc.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 02:04:11 2010
 *
 * @brief  @todo document
 *
 * @todo document
 */

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/adc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "config.h"

#include "driver/adc.h"

#include "driver/led.h"
#include "pwm/pwm.h"
#include "comm_tim.h"
#include "gprot.h"

/**
 * ADC data instance
 */
struct adc_data adc_data;

/**
 * ADC output trigger
 *
 * Can be used to trigger actions in the userspace every time new data is
 * available from the ADC.
 */
volatile bool adc_new_data_trigger;

/**
 * @todo document
 *
 */
void adc_init(void)
{
	NVIC_InitTypeDef nvic;
	GPIO_InitTypeDef gpio;
	ADC_InitTypeDef adc;

	/* enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Configure and enable ADC interrupt */
	nvic.NVIC_IRQChannel = ADC1_2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* GPIOA: ADC Channel 0, 1, 2, 3 as analog input
	 * Ch 0 -> BEMF/I_Sense of PHASE A
	 * Ch 1 -> BEMF/I_Sense of PHASE B
	 * Ch 2 -> BEMF/I_Sense of PHASE C
	 * Ch 3 -> Battery Voltage
	 * Ch 9 -> Global Current
	 */
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &gpio);

	adc_data.phase_voltage = 0;
	adc_data.half_battery_voltage = 0;
	adc_data.global_current = 0;

	/* Configure ADC1 */
	adc.ADC_Mode = ADC_Mode_Independent;
	adc.ADC_ScanConvMode = ENABLE;
	adc.ADC_ContinuousConvMode = DISABLE;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfChannel = 0;
	ADC_Init(ADC1, &adc);

	ADC_InjectedSequencerLengthConfig(ADC1, 3);

	ADC_InjectedChannelConfig(ADC1, ADC_CHANNEL_C, 1,
				  ADC_SampleTime_41Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_CHANNEL_HALF_BATTERY_VOLTAGE, 2,
				  ADC_SampleTime_41Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_CHANNEL_GLOBAL_CURRENT, 3,
				  ADC_SampleTime_41Cycles5);

	ADC_ExternalTrigInjectedConvConfig(ADC1,
					   ADC_ExternalTrigInjecConv_T1_CC4);

	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

	/* Enable ADC1 JEOC interrupt */
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);

	/* Check the end of ADC1 reset calibration */
	while (ADC_GetResetCalibrationStatus(ADC1)) ;

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);

	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1)) ;

	/* Enable ADC1 External Trigger */
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);

}

/**
 * @todo document
 *
 * @param channel @todo document
 */
void adc_set(u8 channel)
{

	ADC_ExternalTrigInjectedConvCmd(ADC1, DISABLE);

	ADC_InjectedChannelConfig(ADC1, channel, 1, ADC_SampleTime_28Cycles5);

	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);
}

/**
 * @todo document
 *
 */
void adc1_2_irq_handler(void)
{
	ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);

	comm_tim_capture_time();

	adc_data.phase_voltage =
	    ADC_GetInjectedConversionValue(ADC1, ADC_PHASE_VOLTAGE);
	adc_data.half_battery_voltage =
	    ADC_GetInjectedConversionValue(ADC1, ADC_HALF_BATTERY_VOLTAGE);
	adc_data.global_current =
	    ADC_GetInjectedConversionValue(ADC1, ADC_GLOBAL_CURRENT);

	adc_new_data_trigger = true;
}
