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

#define PWM_VALUE 1023;

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |
                           RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC, ENABLE);

}

void nvic_init(void){
    NVIC_InitTypeDef nvic;

    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    /* Enable TIM1 interrupt */
    nvic.NVIC_IRQChannel = TIM1_TRG_COM_IRQChannel;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    /* Set SysTick handler */
    NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 0);
}

void gpio_init(void){
    GPIO_InitTypeDef gpio;

    /* GPIOA: TIM1 channel 1, 2 and 3 as alternate function
       push-pull */
    gpio.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    /* GPIOB: TIM1 channel 1N, 2N and 3N as alternate function
     * push-pull
     */
    gpio.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &gpio);

    /* GPIOB: BKIN pin as floating input*/
    gpio.GPIO_Pin   = GPIO_Pin_12;
    gpio.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio);

    /* GPIOC: LED pin as output push-pull */
    GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);
    gpio.GPIO_Pin =  GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);
}

void sys_tick_init(void){
    /* Select HCLK as clock source */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    /* Generate SysTick interrupt every 100ms (with HCLK=72MHz) */
    SysTick_SetReload(7200000);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}

void pwm_init(void){
    TIM_TimeBaseInitTypeDef tim_base;
    TIM_OCInitTypeDef       tim_oc;
    TIM_BDTRInitTypeDef     tim_bdtr;

    /* Time base configuration */
    tim_base.TIM_Period = 1999;
    tim_base.TIM_Prescaler = 0;
    tim_base.TIM_ClockDivision = 0;
    tim_base.TIM_CounterMode = TIM_CounterMode_Up;
    tim_base.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &tim_base);

    /* TIM1 channel 1, 2 and 3 settings */
    tim_oc.TIM_OCMode       = TIM_OCMode_Timing;
    tim_oc.TIM_OutputState  = TIM_OutputState_Enable;
    tim_oc.TIM_OutputNState = TIM_OutputNState_Enable;
    tim_oc.TIM_Pulse        = 500;//PWM_VALUE;
    tim_oc.TIM_OCPolarity   = TIM_OCPolarity_High;
    tim_oc.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    tim_oc.TIM_OCIdleState  = TIM_OCIdleState_Set;
    tim_oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;

    TIM_OC1Init(TIM1, &tim_oc);
    TIM_OC2Init(TIM1, &tim_oc);
    TIM_OC3Init(TIM1, &tim_oc);

    /* Automatic Output enable, break, dead time and lock configuration */
    tim_bdtr.TIM_OSSRState       = TIM_OSSRState_Enable;
    tim_bdtr.TIM_OSSIState       = TIM_OSSIState_Enable;
    tim_bdtr.TIM_LOCKLevel       = TIM_LOCKLevel_OFF;
    tim_bdtr.TIM_DeadTime        = 1;
    tim_bdtr.TIM_Break           = TIM_Break_Disable;
    tim_bdtr.TIM_BreakPolarity   = TIM_BreakPolarity_High;
    tim_bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

    TIM_BDTRConfig(TIM1, &tim_bdtr);

    TIM_CCPreloadControl(TIM1, ENABLE);

    /* Enable COM interrupt */
    TIM_ITConfig(TIM1, TIM_IT_COM, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* Main output enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

int main(void){

    rcc_init(); /* system clocks init */
    nvic_init(); /* interrupt vector unit init */
    gpio_init();
    sys_tick_init();
    pwm_init();

    while(1);
}
