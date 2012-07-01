/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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
 * @file   driver/can.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date
 *
 * @brief  CAN driver implementation
 *
 */

#include "config.h"

#include <stdint.h>
#include <string.h>

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/nvic.h>

#include "driver/can.h"

#include "led.h"

struct can_tx_msg {
	u32 id;
	bool ide;
        bool rtr;
	u8 dlc;
	u8 data[8];
};

struct can_rx_msg {
	u32 id;
        bool ide;
	bool rtr;
	u8 dlc;
	u8 data[8];
	u32 fmi;
};

struct can_tx_msg can_tx_msg;
struct can_rx_msg can_rx_msg;

//void _can_run_rx_callback(uint32_t id, uint8_t *buf, uint8_t len);

void can_setup(void)
{

	/* Enable peripheral clocks. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_CANEN);

	/* Configure CAN pin: RX */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN_RX);
	gpio_set(GPIOA, GPIO_CAN_RX);

	/* Configure CAN pin: TX */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN_TX);

	/* NVIC configuration */
	nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
	nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 1);

	/* CAN register init */
	can_reset(CAN1);

	/* CAN cell init */
	if (can_init(CAN1,
		     false,           /* TTCM: Time triggered comm mode? */
		     true,            /* ABOM: Automatic bus-off management? */
		     false,           /* AWUM: Automatic wakeup mode? */
		     false,           /* NART: No automatic retransmission? */
		     false,           /* RFLM: Receive FIFO locked mode? */
		     false,           /* TXFP: Transmit FIFO priority? */
		     CAN_BTR_SJW_1TQ,
		     CAN_BTR_TS1_3TQ,
		     CAN_BTR_TS2_4TQ,
		     12))             /* BRP+1: Baud rate prescaler */
	{
		ON(LED_RED);
		OFF(LED_GREEN);
		OFF(LED_BLUE);
		OFF(LED_ORANGE);

		/* Die because we failed to initialize. */
		while (1)
			__asm__("nop");
	}

	/* CAN filter init */
	can_filter_id_mask_32bit_init(CAN1,
				      0,     /* Filter ID */
				      0,     /* CAN ID */
				      0,     /* CAN ID mask */
				      0,     /* FIFO assignment (here: FIFO0) */
				      true); /* Enable the filter. */

	/* transmit struct init */
	can_tx_msg.id = 0x0;
	can_tx_msg.rtr = false;
#ifdef CAN__USE_EXT_ID
	can_tx_msg.ide = true;
#else
	can_tx_msg.ide = false;
#endif
	can_tx_msg.dlc = 1;

	can_enable_irq(CAN1, CAN_IER_FMPIE0);
}

int can_trans(uint32_t id, const uint8_t *buf, uint8_t len)
{
	if(len > 8){
		return -1;
	}

	can_tx_msg.id = id;
	can_tx_msg.dlc = len;

	memcpy(can_tx_msg.data, buf, len);

	return can_transmit(CAN1,
			    can_tx_msg.id,
			    can_tx_msg.ide,
			    can_tx_msg.rtr,
			    can_tx_msg.dlc,
			    can_tx_msg.data);
}

void usb_lp_can_rx0_isr(void)
{
	static int delay = 100;

	can_receive(CAN1, 0, false,
		    &can_rx_msg.id,
		    &can_rx_msg.ide,
		    &can_rx_msg.rtr,
		    &can_rx_msg.fmi,
		    &can_rx_msg.dlc,
		    can_rx_msg.data);

	can_fifo_release(CAN1, 0);


	if (can_rx_msg.data[0] & 1) {
		ON(LED_ORANGE);
	} else {
		OFF(LED_ORANGE);
	}

	if (can_rx_msg.data[0] & 2) {
		ON(LED_GREEN);
	} else {
		OFF(LED_GREEN);
	}

	if(delay == 0) {
		delay = 100;
		TOGGLE(LED_RED);
	}else{
		delay--;
	}
		TOGGLE(LED_BLUE);

	//_can_run_rx_callback(can_rx_msg.id, can_rx_msg.data, can_rx_msg.dlc);

}

