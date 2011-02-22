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
 * @brief  ADC peripheral hardware handling implementation.
 *
 * The ADC is being used for a variety of sensory inputs.
 *
 * @todo This code should be divided more into hardware specific and
 * application specific code.
 */

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/adc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "config.h"

#include "driver/adc.h"

#include "driver/led.h"
#include "pwm/pwm.h"
#include "comm_tim.h"
#include "gprot.h"
#include "driver/sys_tick.h"

void adc_conv_trigger(int id);

/**
 * ADC data instance
 */
struct adc_data adc_data;

/**
 * Initialize the ADC peripherals and internal state of the driver
 */
void adc_init(void)
{
	NVIC_InitTypeDef nvic;
	GPIO_InitTypeDef gpio;
	ADC_InitTypeDef adc;

	/* enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			       RCC_APB2Periph_ADC1, ENABLE);

	/* Configure and enable ADC interrupt */
	nvic.NVIC_IRQChannel = ADC1_2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* GPIOA: ADC Channel 0, 1, 2, 3 as analog input
	 * Ch 3 -> Battery Voltage
	 * Ch 4 -> Current
	 * Ch 5 -> Temperature
	 */
	gpio.GPIO_Pin =
		GPIO_Pin_3 |
		GPIO_Pin_4 |
		GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	adc_data.battery_voltage = 0;
	adc_data.current = 0;
	adc_data.temp = 0;

	/* Configure ADC1 */
	adc.ADC_Mode = ADC_Mode_Independent;
	adc.ADC_ScanConvMode = ENABLE;
	adc.ADC_ContinuousConvMode = ENABLE;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfChannel = 0;
	ADC_Init(ADC1, &adc);

	ADC_InjectedSequencerLengthConfig(ADC1, 3);

	ADC_InjectedChannelConfig(ADC1, ADC_CHANNEL_BATTERY_VOLTAGE, 1,
				  ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_CHANNEL_CURRENT, 2,
				  ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_CHANNEL_TEMP, 3,
				  ADC_SampleTime_239Cycles5);

        /* ADC1 injected external trigger configuration */
        ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

	/* Enable ADC1 JEOC interrupt */
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);

	/* Check the end of ADC1 reset calibration */
	while (ADC_GetResetCalibrationStatus(ADC1) == SET) ;

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);

	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1) == SET) ;

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);

	/* Register adc as a timed callback */
	sys_tick_timer_register(adc_conv_trigger, 1000);
}

/**
 * ADC1 interrupt hander
 */
void adc1_2_isr(void)
{
	ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);

	adc_data.battery_voltage =
		ADC_GetInjectedConversionValue(ADC1, ADC_BATTERY_VOLTAGE);
	adc_data.current =
		ADC_GetInjectedConversionValue(ADC1, ADC_CURRENT);
	adc_data.temp =
		ADC_GetInjectedConversionValue(ADC1, ADC_TEMP);

	adc_data.trigger = true;
}

/**
 * Trigger timer callback
 */
void adc_conv_trigger(int id) {

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
}
