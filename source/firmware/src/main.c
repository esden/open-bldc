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
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>

void rcc_init(void){
    /* Initialize the microcontroller system. Initialize clocks. */
    SystemInit();

    /* TIM1, GPIOA, GPIOB and GPIOC clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);

}

void nvic_init(void){
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

}

void gpio_init(void){
    GPIO_InitTypeDef gpio;

    /* GPIOC: LED pin as output push-pull */
    GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
    gpio.GPIO_Pin =  GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

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

    while(1){
        GPIOB->BRR |= 0x00001000;
        my_delay(1000000);
        //USART_SendData(USART3, 'A');
        GPIOB->BSRR |= 0x00001000;
        my_delay(1000000);
    }
}
