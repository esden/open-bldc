/*
 * Open-BLDC - Open BruschLess DC Motor Controller
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
#include <stm32/adc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include "config.h"
#include "soft_timer.h"

#include "adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

vu16 adc_val[32];
vu8 adc_rising = ADC_RISING;
vu16 adc_old_value = 2;
vu8 adc_count = 0;
vu16 adc_delay_count = 0;

void adc_rcc_init(void){
    /* enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* enable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void adc_nvic_init(void){
    NVIC_InitTypeDef nvic;

    /* Configure and enable ADC interrupt */
    nvic.NVIC_IRQChannel = ADC1_2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void adc_gpio_init(void){
    GPIO_InitTypeDef gpio;

    /* GPIOA: ADC Channel 1 as analog input */
    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &gpio);
}

void adc_init(void){
    ADC_InitTypeDef adc;

    adc_val[1]=0;
    adc_val[2]=0;
    adc_val[3]=0;

    /* Configure ADC1 */
    adc.ADC_Mode = ADC_Mode_Independent;
    adc.ADC_ScanConvMode = DISABLE;
    adc.ADC_ContinuousConvMode = DISABLE;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &adc);

    /* Configure ADC1 regular channel1 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_28Cycles5);

    /* Enable ADC1 EOC interrupt */
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADC1);

    /* Check the end of ADC1 reset calibration */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);

    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));

    /* Enable ADC1 External Trigger */
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);

}

void adc_set(u8 channel, u32 trig_channel, u8 rising, u8 clear_comm){
    u32 tmpreg;

    adc_rising = rising;

    //ADC_RegularChannelConfig(ADC1, channel, 1,
    //                         ADC_SampleTime_28Cycles5);

    ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);

    ADC1->SQR3 = channel;

    tmpreg = ADC1->CR2;
    tmpreg &= 0xFFF1FFFF; //CR2_EXTSEL_Reset;
    tmpreg |= trig_channel;
    ADC1->CR2 = tmpreg;

    if(clear_comm){
#if ADC_COMM_TRIGGER == 1
        GPIOC->BRR |= 0x00001000;
#endif
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
        adc_old_value = 0;
        adc_count = 0;
        adc_delay_count = 0;
    }

}

void adc1_2_irq_handler(void){
    u16 new_value = ADC_GetConversionValue(ADC1);

    if(adc_delay_count > 10){
        if(adc_rising){
            if(new_value <= (adc_old_value+6)){
                if(adc_count > 1){
                    if(!soft_comm)
                        TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
#if ADC_COMM_TRIGGER == 1
                    GPIOC->BSRR |= 0x00001000;
#endif
                    ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
                }else adc_count++;
            }else adc_count = 0;
            adc_old_value = new_value;
        }else{
            if(new_value < 1){
                if(adc_count > 1){
                    if(!soft_comm)
                        TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
#if ADC_COMM_TRIGGER == 1
                    GPIOC->BSRR |= 0x00001000;
#endif
                    ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
                }else adc_count++;
            }else adc_count = 0;
        }
    }else adc_delay_count++;

    adc_val[0] = new_value;

}
