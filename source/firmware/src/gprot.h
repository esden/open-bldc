/*
 * Open-BLDC - Open BrushLess DC Motor Controller
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

#ifndef __GPROT_H
#define __GPROT_H

#define GPROT_FLAG_REG_ADDR 0
#define GPROT_PWM_OFFSET_REG_ADDR 1
#define GPROT_PWM_VAL_REG_ADDR 2
#define GPROT_COMM_TIM_FREQ_REG_ADDR 3
#define GPROT_ADC_LEVEL_RISING_REG_ADDR 4
#define GPROT_ADC_LEVEL_FALLING_REG_ADDR 5

void gprot_init();

#endif /* __GPROT_H */
