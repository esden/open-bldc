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
#define ADC_CHANNEL_BATTERY_VOLTAGE ADC_Channel_3
#define ADC_CHANNEL_CURRENT ADC_Channel_4
#define ADC_CHANNEL_TEMP ADC_Channel_5

#define ADC_BATTERY_VOLTAGE ADC_InjectedChannel_1
#define ADC_CURRENT ADC_InjectedChannel_2
#define ADC_TEMP ADC_InjectedChannel_3
/** @} */

/**
 * ADC output data
 */
struct adc_data {
	bool trigger; /**< New data arrived trigger */
	u16 battery_voltage; /**< Raw half battery voltage value */
	u16 current; /**< Raw global current value */
	u16 temp; /**< RAW temperature measurement */
};

extern struct adc_data adc_data;
extern volatile bool adc_new_data_trigger;

void adc_init(void);
void adc_set(u8 channel);

#endif /* __ADC_H */
