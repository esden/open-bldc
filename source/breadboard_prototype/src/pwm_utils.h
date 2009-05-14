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

#ifndef __PWM_UTILS_H
#define __PWM_UTILS_H

#define PWM_PHASE_A TIM_Channel_1
#define PWM_PHASE_B TIM_Channel_2
#define PWM_PHASE_C TIM_Channel_3

void pwm_trigger(uint16_t zone) __attribute__((always_inline));
void pwm_set_____hi(uint16_t phase) __attribute__((always_inline));
void pwm_set_____lo(uint16_t phase) __attribute__((always_inline));
void pwm_set_pwm_hi(uint16_t phase) __attribute__((always_inline));
void pwm_set_pwm_lo(uint16_t phase) __attribute__((always_inline));
void pwm_set____off(uint16_t phase) __attribute__((always_inline));

#endif /* __PWM_UTILS_H */
