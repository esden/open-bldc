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

#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

extern volatile int comm_timer_reload;

void nmi_exception(void);
void hard_fault_exception(void);
void mem_manage_exception(void);
void bus_fault_exception(void);
void usage_fault_exception(void);
void debug_monitor(void);
void svc_handler(void);
void pend_svc(void);
void sys_tick_handler(void);
void wwdg_irq_handler(void);
void pvd_irq_handler(void);
void tamper_irq_handler(void);
void rtc_irq_handler(void);
void flash_irq_handler(void);
void rcc_irq_handler(void);
void exti0_irq_handler(void);
void exti1_irq_handler(void);
void exti2_irq_handler(void);
void exti3_irq_handler(void);
void exti4_irq_handler(void);
void dma1_channel1_irq_handler(void);
void dma1_channel2_irq_handler(void);
void dma1_channel3_irq_handler(void);
void dma1_channel4_irq_handler(void);
void dma1_channel5_irq_handler(void);
void dma1_channel6_irq_handler(void);
void dma1_channel7_irq_handler(void);
void adc1_2_irq_handler(void);
void usb_hp_can_tx_irq_handler(void);
void usb_lp_can_rx0_irq_handler(void);
void can_rx1_irq_handler(void);
void can_sce_irq_handler(void);
void exti9_5_irq_handler(void);
void tim1_brk_irq_handler(void);
void tim1_up_irq_handler(void);
// void tim1_trg_com_irq_handler(void); // moved to pwm module
void tim1_cc_irq_handler(void);
void tim2_irq_handler(void);
void tim3_irq_handler(void);
void tim4_irq_handler(void);
void i2c1_ev_irq_handler(void);
void i2c1_er_irq_handler(void);
void i2c2_ev_irq_handler(void);
void i2c2_er_irq_handler(void);
void spi1_irq_handler(void);
void spi2_irq_handler(void);
void usart1_irq_handler(void);
void usart2_irq_handler(void);
//void usart3_irq_handler(void); // moved to usart module
void exti15_10_irq_handler(void);
void rtc_alarm_irq_handler(void);
void usb_wake_up_irq_handler(void);
void tim8_brk_irq_handler(void);
void tim8_up_irq_handler(void);
void tim8_trg_com_irq_handler(void);
void tim8_cc_irq_handler(void);
void adc3_irq_handler(void);
void fsmc_irq_handler(void);
void sdio_irq_handler(void);
void tim5_irq_handler(void);
void spi3_irq_handler(void);
void uart4_irq_handler(void);
void uart5_irq_handler(void);
void tim6_irq_handler(void);
void tim7_irq_handler(void);
void dma2_channel1_irq_handler(void);
void dma2_channel2_irq_handler(void);
void dma2_channel3_irq_handler(void);
void dma2_channel4_5_irq_handler(void);

#endif /* INTERRUPTS_H */
