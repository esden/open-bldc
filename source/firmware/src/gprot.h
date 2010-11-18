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

#ifndef __GPROT_H
#define __GPROT_H

/** @{ */
/**
 * Governor register address definition.
 */
#define GPROT_FLAG_REG_ADDR 0
#define GPROT_PWM_OFFSET_REG_ADDR 1
#define GPROT_PWM_VAL_REG_ADDR 2
#define GPROT_COMM_TIM_FREQ_REG_ADDR 3
#define GPROT_COMM_TIM_SPARK_ADVANCE_REG_ADDR 4
#define GPROT_COMM_TIM_DIRECT_CUTOFF_REG_ADDR 5
#define GPROT_COMM_TIM_IIR_POLE_REG_ADDR 6
#define GPROT_NEW_CYCLE_TIME 7
#define GPROT_CPU_LOAD 8
#define GPROT_CPU_LOAD_MAX 9
#define GPROT_CPU_LOAD_MIN 10
#define GPROT_ADC_BATTERY_VOLTAGE_REG_ADDR 11
#define GPROT_ADC_CURRENT_REG_ADDR 12
#define GPROT_ADC_TEMPERATURE_REG_ADDR 13
/** @} */

void gprot_init();

#endif /* __GPROT_H */
