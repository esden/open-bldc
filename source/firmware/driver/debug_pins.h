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

#ifndef __DEBUG_PINS_H
#define __DEBUG_PINS_H

#include "config.h"

#include "macro_utils.h"

#ifdef DP__USE_ENCODER

#define DP_ENC_A_PORT GPIOA
#define DP_ENC_A_PIN 6

#define DP_ENC_B_PORT GPIOA
#define DP_ENC_B_PIN 7

#endif

#ifdef DP__USE_EXT_I2C

#define DP_EXT_SCL_PORT GPIOB
#define DP_EXT_SCL_PIN 8

#define DP_EXT_SDA_PORT GPIOB
#define DP_EXT_SDA_PIN 9

#endif

void debug_pins_init(void);

#endif /* __DP_EXT_PINS_H */
