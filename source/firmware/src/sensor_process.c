/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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
 * @file   sensor_process.c
 * @author Piotr Esden-Tempski <esden@esden.net>
 * @date   Tue Aug 17 00:29:15 2010
 *
 * @brief  Sensory input postprocessing user land process.
 *
 * Implementation of the sensor user land process which is being called each
 * time new sensor data arrive through the interrupt system.
 */

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "gprot.h"
#include "driver/led.h"
#include "driver/adc.h"
#include "gprot.h"

#include "sensor_process.h"

/**
 * Parameters for sensor data post processing
 */
struct sensor_params {
	/**
	 * Phase voltage post processing parameters
	 */
	struct pv {
		s32 offset; /**< how much to offset the value */
		u32 iir;    /**< IIR filter value */
	} pv;
	/**
	 * Supply rail voltage post processing parameters
	 */
	struct bv {
		s32 offset; /**< how much to offset the value */
		u32 iir;    /**< IIR filter value */
	} bv;
	/**
	 * Global controller current
	 */
	struct c {
		s32 offset;	 /**< The value of zero */
		u32 iir;	 /**< IIR filter value */
	} c;
	/**
	 * Powerstage temperature
	 */
	struct t {
		s32 offset; /**< Zero temperature offset value */
		u32 iir;	 /**< IIR filter value */
	} t;
};

/**
 * Sensor value struct instance
 */
struct sensors sensors;

/**
 * Sensor post processing parameters instance
 */
static struct sensor_params sensor_params;

/**
 * Sensor process Trigger
 */
volatile bool *sensor_process_trigger;

/**
 * Generate debug output flag
 */
static int sensor_trigger_debug_output;

/**
 * Infinite Impulse Response filter calculation
 */
#define SENSOR_OFFSET_IIR(VALUE, NEW_VALUE, OFFSET, IIR) \
	(((VALUE * IIR) + (NEW_VALUE + OFFSET)) / (IIR + 1))

/**
 * Sensor process initializer.
 *
 * Resets all global presets.
 */
void sensor_process_init(void)
{

	(void)gpc_setup_reg(GPROT_ADC_BATTERY_VOLTAGE_REG_ADDR,
			    (u16 *) & (sensors.battery_voltage));
	(void)gpc_setup_reg(GPROT_ADC_CURRENT_REG_ADDR,
			    (u16 *) & (sensors.current));
	(void)gpc_setup_reg(GPROT_ADC_TEMPERATURE_REG_ADDR,
			    (u16 *) & (sensors.temp));

	sensor_process_trigger = &adc_data.trigger;

	sensors.battery_voltage = 0;
	sensors.current = 0;
	sensors.temp = 0;

	sensor_params.bv.offset = 0;
	sensor_params.bv.iir = 20;
	sensor_params.c.offset = 0;
	sensor_params.c.iir = 20;
	sensor_params.t.offset = 0;
	sensor_params.t.iir = 20;

	sensor_trigger_debug_output = 0;
}

/**
 * Resets all sensor data to default values.
 */
void sensor_process_reset(void)
{
	sensors.battery_voltage = 0;
	sensors.current = 0;
	sensors.temp = 0;
}

/**
 * The main periodic process implementation.
 *
 * This function is being called every time new sensor data arrives through the
 * interrupt system.
 */
void run_sensor_process(void)
{
	u16 battery_voltage = adc_data.battery_voltage;
	u16 current = adc_data.current;
	u16 temp = adc_data.temp;

	TOGGLE(LED_RED);
	/* Calculate battery voltage */
	sensors.battery_voltage =
		SENSOR_OFFSET_IIR(sensors.battery_voltage, battery_voltage,
				sensor_params.bv.offset,
				sensor_params.bv.iir);

	/* Calculate global current */
	sensors.current =
		SENSOR_OFFSET_IIR(sensors.current, current,
				sensor_params.c.offset,
				sensor_params.c.iir);

	/* Calculate power stage temperature */
	sensors.temp =
		SENSOR_OFFSET_IIR(sensors.temp, temp,
				sensor_params.t.offset,
				sensor_params.t.iir);

	if (sensor_trigger_debug_output == 10) {
		sensor_trigger_debug_output = 0;
		(void)gpc_register_touched(GPROT_ADC_BATTERY_VOLTAGE_REG_ADDR);
		(void)gpc_register_touched(GPROT_ADC_CURRENT_REG_ADDR);
		(void)gpc_register_touched(GPROT_ADC_TEMPERATURE_REG_ADDR);
	} else {
		sensor_trigger_debug_output++;
	}

}
