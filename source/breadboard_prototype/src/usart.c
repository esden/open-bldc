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

#include <stm32/lib.h>

#include "pwm.h"
#include "soft_timer.h"

#include "usart.h"

void usart_rcc_init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

void usart_nvic_init(void){
    NVIC_InitTypeDef nvic;

    /* Enable the USART3 interrupts */
    nvic.NVIC_IRQChannel = USART3_IRQChannel;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void usart_gpio_init(void){
    GPIO_InitTypeDef gpio;

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

}

void usart_init(void){
    USART_InitTypeDef usart;

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
    //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

    /* Enable the USART3 */
    USART_Cmd(USART3, ENABLE);
}

void usart3_irq_handler(void){
    char buff;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
        buff = USART_ReceiveData(USART3);
	switch(buff){
        case 'a':
            if(comm_timer_reload > 0) comm_timer_reload-=1;
            SysTick_SetReload(comm_timer_reload);
            break;
        case 'b':
            if(comm_timer_reload < 200000) comm_timer_reload+=1;
            SysTick_SetReload(comm_timer_reload);
            break;
        case 'c':
            if(pwm_val > 0) pwm_val-=1;
            break;
        case 'd':
            if(pwm_val < 1989) pwm_val+=1;
            break;
        case 'e':
            if(comm_timer_reload > 72*2) comm_timer_reload-=72*2;
            SysTick_SetReload(comm_timer_reload);
            break;
        case 'f':
            if(comm_timer_reload < 200000-72*2) comm_timer_reload+=72*2;
            SysTick_SetReload(comm_timer_reload);
            break;
	}
    }
#if 0
    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET){
        USART_SendData(USART3, 'A');
        if(led_state){
            GPIOC->BRR |= 0x00001000;
            led_state = 0;
        }else{
            GPIOC->BSRR |= 0x00001000;
            led_state = 1;
        }
    }
#endif
}
