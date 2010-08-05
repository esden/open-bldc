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

#include <stm32/rcc.h>
#include <stm32/gpio.h>

void led_init(void)
{
	GPIO_InitTypeDef gpio;

	/* GPIOA, GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
			       RCC_APB2Periph_GPIOC, ENABLE);

	/* GPIOB: Orange and Red LED pin as output push-pull */
	GPIO_WriteBit(GPIOB, GPIO_Pin_2 | GPIO_Pin_12, Bit_SET);
	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	/* GPIOC: Blue and Green LED pin as output push-pull */
	GPIO_WriteBit(GPIOC, GPIO_Pin_13 | GPIO_Pin_14, Bit_SET);
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);

}
