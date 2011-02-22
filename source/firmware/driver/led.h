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

#ifndef __LED_H
#define __LED_H

#include "macro_utils.h"

#include <libopencm3/stm32/gpio.h>

#define LED_ORANGE_PORT GPIOA
#define LED_ORANGE_PIN GPIO6

#define LED_RED_PORT GPIOB
#define LED_RED_PIN GPIO1

#define LED_GREEN_PORT GPIOA
#define LED_GREEN_PIN  GPIO7

#define LED_BLUE_PORT GPIOB
#define LED_BLUE_PIN GPIO0

void led_init(void);

#endif /* __LED_H */
