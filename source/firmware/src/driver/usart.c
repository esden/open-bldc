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

/**
 * @file   usart.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 02:02:15 2010
 *
 * @brief  TODO
 *
 * TODO
 */

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/usart.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>

#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "usart.h"

#include "led.h"

volatile s16 data_buf;

/**
 * TODO
 *
 */
void usart_init(void)
{
	NVIC_InitTypeDef nvic;
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;

	/* enable clock for USART1 peripherial */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |
			       RCC_APB2Periph_GPIOB |
			       RCC_APB2Periph_AFIO, ENABLE);

	/* Enable the USART1 interrupts */
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* enable USART1 pin software remapping */
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

	/* GPIOB: USART1 Tx push-pull */
	//GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);
	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	/* GPIOB: USART1 Rx pin as floating input */
	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &gpio);

	/* Initialize the usart subsystem */
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1 */
	USART_Init(USART1, &usart);

	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

/**
 * TODO
 *
 */
void usart_enable_send(void)
{
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/**
 * TODO
 *
 */
void usart_disable_send(void)
{
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}

/**
 * TODO
 *
 */
void usart1_irq_handler(void)
{

	//LED_GREEN_TOGGLE();
	/* input (RX) handler */
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		data_buf = USART_ReceiveData(USART1);

		if (!gpc_handle_byte(data_buf)) {
			//LED_GREEN_TOGGLE();
		} else {
			//LED_RED_ON();
		}
	}

	/* output (TX) handler */
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
		if ((data_buf = gpc_pickup_byte()) >= 0) {
			USART_SendData(USART1, data_buf);
			//LED_GREEN_TOGGLE();
		} else {
			usart_disable_send();
		}
	}
}
