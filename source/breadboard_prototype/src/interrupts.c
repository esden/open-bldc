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

#include "interrupts.h"

int bldc_phase = 1;
int led_state = 0;

void nmi_exception(void){
}

void hard_fault_exception(void){
    while(1);
}

void mem_manage_exception(void){
    while(1);
}

void bus_fault_exception(void){
    while(1);
}

void usage_fault_exception(void){
    while(1);
}

void debug_monitor(void){
}

void svc_handler(void){
}

void pend_svc(void){
}

void sys_tick_handler(void){
    /* generate a TIM1 COM event */
    TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
#if 0
    if(led_state){
	    GPIOC->BRR |= 0x00001000;
	    led_state = 0;
    }else{
	    GPIOC->BSRR |= 0x00001000;
	    led_state = 1;
    }
#endif
}

void wwdg_irq_handler(void){
}

void pvd_irq_handler(void){
}

void tamper_irq_handler(void){
}

void rtc_irq_handler(void){
}

void flash_irq_handler(void){
}

void rcc_irq_handler(void){
}

void exti0_irq_handler(void){
}

void exti1_irq_handler(void){
}

void exti2_irq_handler(void){
}

void exti3_irq_handler(void){
}

void exti4_irq_handler(void){
}

void dma1_channel1_irq_handler(void){
}

void dma1_channel2_irq_handler(void){
}

void dma1_channel3_irq_handler(void){
}

void dma1_channel4_irq_handler(void){
}

void dma1_channel5_irq_handler(void){
}

void dma1_channel6_irq_handler(void){
}

void dma1_channel7_irq_handler(void){
}

void adc1_2_irq_handler(void){
}

void usb_hp_can_tx_irq_handler(void){
}

void usb_lp_can_rx0_irq_handler(void){
}

void can_rx1_irq_handler(void){
}

void can_sce_irq_handler(void){
}

void exti9_5_irq_handler(void){
}

void tim1_brk_irq_handler(void){
}

void tim1_up_irq_handler(void){
}

void tim1_trg_com_irq_handler(void){
    TIM_ClearITPendingBit(TIM1, TIM_IT_COM);

#if 1
    if(led_state){
	    GPIOC->BRR |= 0x00001000;
	    led_state = 0;
    }else{
	    GPIOC->BSRR |= 0x00001000;
	    led_state = 1;
    }
#endif

    switch(bldc_phase){
    case 1:
        /* Next step: Step 2 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);

        /*  Channel1 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

        /*  Channel2 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_ForcedAction_InActive );
        TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

        bldc_phase++;
        break;
    case 2:
        /* Next step: Step 3 Configuration ---------------------------- */
        /*  Channel2 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_ForcedAction_InActive);
        TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

        /*  Channel3 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
        TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);

        /*  Channel1 configuration */
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

        bldc_phase++;
        break;
    case 3:
        /* Next step: Step 4 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
        TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);

        /*  Channel2 configuration */
        TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

        /*  Channel1 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_ForcedAction_InActive);
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

        bldc_phase++;
        break;
    case 4:
        /* Next step: Step 5 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);

        /*  Channel1 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_ForcedAction_InActive);
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

        /*  Channel2 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
        TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

        bldc_phase++;
        break;
    case 5:
        /* Next step: Step 6 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_ForcedAction_InActive);
        TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);

        /*  Channel1 configuration */
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

        /*  Channel2 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
        TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

        bldc_phase++;
        break;
    case 6:
        /* Next step: Step 1 Configuration ---------------------------- */
        /*  Channel1 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
        TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

        /*  Channel3 configuration */
        TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_ForcedAction_InActive);
        TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
        TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);

        /*  Channel2 configuration */
        TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
        TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

        bldc_phase=1;
        break;
    }
}

void tim1_cc_irq_handler(void){
}

void tim2_irq_handler(void){
}

void tim3_irq_handler(void){
}

void tim4_irq_handler(void){
}

void i2c1_ev_irq_handler(void){
}

void i2c1_er_irq_handler(void){
}

void i2c2_ev_irq_handler(void){
}

void i2c2_er_irq_handler(void){
}

void spi1_irq_handler(void){
}

void spi2_irq_handler(void){
}

void usart1_irq_handler(void){
}

void usart2_irq_handler(void){
}

void usart3_irq_handler(void){
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

void exti15_10_irq_handler(void){
}

void rtc_alarm_irq_handler(void){
}

void usb_wake_up_irq_handler(void){
}

void tim8_brk_irq_handler(void){
}

void tim8_up_irq_handler(void){
}

void tim8_trg_com_irq_handler(void){
}

void tim8_cc_irq_handler(void){
}

void adc3_irq_handler(void){
}

void fsmc_irq_handler(void){
}

void sdio_irq_handler(void){
}

void tim5_irq_handler(void){
}

void spi3_irq_handler(void){
}

void uart4_irq_handler(void){
}

void uart5_irq_handler(void){
}

void tim6_irq_handler(void){
}

void tim7_irq_handler(void){
}

void dma2_channel1_irq_handler(void){
}

void dma2_channel2_irq_handler(void){
}

void dma2_channel3_irq_handler(void){
}

void dma2_channel4_5_irq_handler(void){
}
