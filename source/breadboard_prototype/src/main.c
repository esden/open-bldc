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
#include "usart.h"

void rcc_init(void){
    ErrorStatus err;

    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    err = RCC_WaitForHSEStartUp();

    if(err == SUCCESS){
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);

        /* PCLK1 = HCLK/4 */
        RCC_PCLK1Config(RCC_HCLK_Div4);

        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08);
    }

    /* TIM1, GPIOA, GPIOB and GPIOC clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_AFIO, ENABLE);

    pwm_rcc_init();
    usart_rcc_init();
}

void nvic_init(void){
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    /* Set SysTick handler */
    NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 0);

    pwm_nvic_init();
    usart_nvic_init();
}

void gpio_init(void){
    GPIO_InitTypeDef gpio;

    /* GPIOC: LED pin as output push-pull */
    GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);
    gpio.GPIO_Pin =  GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);

    pwm_gpio_init();
    usart_gpio_init();
}

void sys_tick_init(void){
    /* Select HCLK as clock source */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    /* Generate SysTick interrupt every 100us (with HCLK=72MHz) */
    SysTick_SetReload(7200);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}

void my_delay(unsigned long delay ){

    while(delay){
        delay--;
    }
}

int main(void){

    rcc_init(); /* system clocks init */
    nvic_init(); /* interrupt vector unit init */
    gpio_init();
    usart_init();
    sys_tick_init();
    pwm_init();

    while(1){
#if 0
        GPIOC->BRR |= 0x00001000;
        my_delay(1000000);
        USART_SendData(USART3, 'A');
        GPIOC->BSRR |= 0x00001000;
        my_delay(1000000);
#endif
    }
}
