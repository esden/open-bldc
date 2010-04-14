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

#ifndef __COMM_TIM_H
#define __COMM_TIM_H

extern volatile uint16_t comm_tim_freq;
extern volatile s16 comm_tim_spark_advance;

void comm_tim_init(void);
void comm_tim_on(void);
void comm_tim_off(void);
void comm_tim_remember(void);
void comm_tim_set_next_comm(void);
void tim2_irq_handler(void);

#endif /* __COMM_TIM_H */
