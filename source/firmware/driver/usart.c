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
 * @brief  USART driver implementation
 *
 */

#include "config.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/nvic.h>

#include "types.h"

#include <lg/gpdef.h>
#include <lg/gprotc.h>

#include "usart.h"

#include "led.h"

/**
 * Data buffer used for incoming and outgoing data.
 */
static volatile s16 data_buf;

/**
 * USART driver initialization.
 */
void usart_init(void)
{

	/* enable clock for USART1 peripherial */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
        rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN);
        rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_USART1EN);

	/* Enable the USART1 interrupts */
	nvic_enable_irq(NVIC_USART1_IRQ);

	/* enable USART1 pin software remapping */
	AFIO_MAPR |= AFIO_MAPR_USART1_REMAP;

	/* GPIOB: USART1 Tx push-pull */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_RE_TX);

	/* GPIOB: USART1 Rx pin as floating input */
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                      GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RE_RX);

	/* Initialize the usart subsystem */
	usart_set_baudrate(USART1, USART__BAUD);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_RX | USART_MODE_TX);

	/* Enable USART1 Receive and Transmit interrupts */
	USART_CR1(USART1) |= USART_CR1_RXNEIE;
	//USART_CR1(USART1) |= USART_CR1_TXEIE;

	/* Enable the USART1 */
	usart_enable(USART1);
}

/**
 * Enable USART send interrupt.
 */
void usart_enable_send(void)
{
	USART_CR1(USART1) |= USART_CR1_TXEIE;
}

/**
 * Disable USART send interrupt.
 */
void usart_disable_send(void)
{
	USART_CR1(USART1) &= ~USART_CR1_TXEIE;
}

/**
 * USART interrupt handler.
 */
void usart1_isr(void)
{

	//TOGGLE(GREEN);
	/* input (RX) handler */
	if ((USART_SR(USART1) & USART_SR_RXNE) != 0) {
		data_buf = usart_recv(USART1);

		if (gpc_handle_byte((u8)data_buf) != 0) {
			//LED_GREEN_TOGGLE();
		} else {
			//LED_RED_ON();
		}
	}

	/* output (TX) handler */
	if ((USART_SR(USART1) & USART_SR_TXE) != 0) {
		if ((data_buf = gpc_pickup_byte()) >= 0) {
			usart_send(USART1, (uint16_t)data_buf);
			//LED_GREEN_TOGGLE();
		} else {
			usart_disable_send();
		}
	}
}
