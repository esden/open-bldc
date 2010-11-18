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

#ifndef __SENSOR_PROCESS_H
#define __SENSOR_PROCESS_H

/**
 * Sensor values
 */
struct sensors {
	s16 battery_voltage;      /**< Supply rail voltage */
	s16 current;	          /**< Global controller current */
	s16 temp;                 /**< Powerstage temperature */
};

extern struct sensors sensors;

extern volatile bool *sensor_process_trigger;

void sensor_process_init(void);
void sensor_process_reset(void);
void run_sensor_process(void);

#endif /* __SENSOR_PROCESS_H */
