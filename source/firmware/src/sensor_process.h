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

struct sensors {
	u16 phase_voltage;
	u32 half_battery_voltage;
	u16 global_current;
};

struct sensor_params {
	struct pv {
		s32 offset;
		u32 iir;
	} pv;
	struct hbv {
		s32 offset;
		u32 iir;
	} hbv;
	struct gc {
		s32 zero_current_offset;
		s32 zero_current;
		u32 iir;
	} gc;
};

extern struct sensors sensors;
extern struct sensor_params sensor_params;

void sensor_process_init(void);
void sensor_process_reset(void);
void run_sensor_process(void);

#endif /* __SENSOR_PROCESS_H */
