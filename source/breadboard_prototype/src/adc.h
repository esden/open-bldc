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

#ifndef __ADC_H
#define __ADC_H

#define ADC_CHANNEL_A ADC_Channel_1
#define ADC_CHANNEL_B ADC_Channel_2
#define ADC_CHANNEL_C ADC_Channel_3

#define ADC_TRIG_CHANNEL_A ADC_ExternalTrigConv_T1_CC1
#define ADC_TRIG_CHANNEL_B ADC_ExternalTrigConv_T1_CC2
#define ADC_TRIG_CHANNEL_C ADC_ExternalTrigConv_T1_CC3

#define ADC_FALLIN 0
#define ADC_RISING 1

#define ADC_NO_CLEAR_COMM 0
#define ADC_CLEAR_COMM 1

extern volatile uint16_t adc_val[32];

void adc_rcc_init(void);
void adc_nvic_init(void);
void adc_gpio_init(void);
void adc_init(void);
void adc_set(uint8_t channel, uint32_t trig_channel, uint8_t rising, uint8_t clear_comm);
void adc1_2_irq_handler(void);

#endif /* __ADC_H */
