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
 * @file   debug_pins.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Sun Sep 12 16:31:37 2010
 *
 * @brief  Use i2c pins as toggeling debug output.
 *
 */

#include <stm32/rcc.h>
#include <stm32/gpio.h>

#include "config.h"

#include "debug_pins.h"

void debug_pins_init(void)
{
	GPIO_InitTypeDef gpio;

	/* GPIOA, GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			       RCC_APB2Periph_GPIOB, ENABLE);

#ifdef DP__USE_ENCODER
#warning "Using encoder pins as debug outputs"
	/* GPIOA: ENCODER_A, ENCODER_B pin as output push-pull */
	GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, Bit_SET);
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
#endif

#ifdef DP__USE_EXT_I2C
#warning "Using I2C pins as debug outputs"
	/* GPIOB: EXT_SCL and EXT_SDA pin as output push-pull */
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
#endif

}
