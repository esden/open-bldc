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

void usart_init(void)
{
    NVIC_InitTypeDef nvic;
    GPIO_InitTypeDef gpio;
    USART_InitTypeDef usart;

    /* enable clock for USART3 peripherial*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    /* Enable the USART3 interrupts */
    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    /* GPIOB: USART3 Tx push-pull
     */
    gpio.GPIO_Pin   = GPIO_Pin_10;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    /* GPIOB: USART3 Rx pin as floating input */
    gpio.GPIO_Pin   = GPIO_Pin_11;
    gpio.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio);

    /* Initialize the usart subsystem */
    usart.USART_BaudRate            = 9600;
    usart.USART_WordLength          = USART_WordLength_8b;
    usart.USART_StopBits            = USART_StopBits_1;
    usart.USART_Parity              = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure USART3 */
    USART_Init(USART3, &usart);

    /* Enable USART3 Receive and Transmit interrupts */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART3, USART_IT_TXE, DISABLE);

    /* Enable the USART3 */
    USART_Cmd(USART3, ENABLE);
}

void usart_enable_send(void)
{
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void usart_disable_send(void)
{
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
}

void usart3_irq_handler(void)
{

    /* input (RX) handler */
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
        data_buf = USART_ReceiveData(USART3);

        if(gpc_handle_byte(data_buf)){
		LED_GREEN_ON();
	}else{
		LED_BLUE_TOGGLE();
	}
    }

    /* output (TX) handler */
    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET){
//            USART_SendData(USART3, 'A');
#if 1
	    if((data_buf = gpc_pickup_byte()) >= 0){
		    USART_SendData(USART3, data_buf);
		    LED_RED_TOGGLE();
	    }else{
		    usart_disable_send();
	    }
#endif
    }
}
