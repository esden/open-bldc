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

#ifndef __ADC_H
#define __ADC_H

/** @{ */
/**
 * ADC Channel to sensor name mappings
 */
#define ADC_CHANNEL_A ADC_Channel_0
#define ADC_CHANNEL_B ADC_Channel_1
#define ADC_CHANNEL_C ADC_Channel_2
#define ADC_CHANNEL_HALF_BATTERY_VOLTAGE ADC_Channel_3
#define ADC_CHANNEL_GLOBAL_CURRENT ADC_Channel_9

#define ADC_PHASE_VOLTAGE ADC_InjectedChannel_1
#define ADC_HALF_BATTERY_VOLTAGE ADC_InjectedChannel_2
#define ADC_GLOBAL_CURRENT ADC_InjectedChannel_3
/** @} */

/**
 * ADC output data
 */
struct adc_data {
	u16 phase_voltage; /**< Raw phase voltage value */
	u16 half_battery_voltage; /**< Raw half battery voltage value */
	u16 global_current; /**< Raw global current value */
};

extern struct adc_data adc_data;
extern volatile bool adc_new_data_trigger;

void adc_init(void);
void adc_set(u8 channel);

#endif /* __ADC_H */
