/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2011 by Piotr Esden-Tempski <piotr@esden.net>
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

#include "types.h"

#include "driver/i2c1.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#define I2C1_SLAVE_ADDR 0x32

/**
 * Initialize i2c1 peripheral
 */
void i2c1_init(void)
{

	/* Enable clocks for i2c1, alternate function and GPIOB peripherals. */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_I2C1EN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN |
				                  RCC_APB2ENR_AFIOEN);

	/* Enable remapping of i2c1. */
	AFIO_MAPR |= AFIO_MAPR_I2C1_REMAP;

	/* Set alternate functions for the SCL and SDA pins of I2C1. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                      GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                      GPIO_I2C1_RE_SCL | GPIO_I2C1_RE_SDA);

	/* Disable the I2C before changing any configuration. */
	i2c_peripheral_disable(I2C1);

	/* APB1 is running at 36MHz. */
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);

	/* 400KHz - I2C Fast Mode */
	i2c_set_fast_mode(I2C1);

	/*
         * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400KHz
         * incl trise -> Thigh= 1600ns; CCR= tlow/tcycle= 0x1C,9;
         * datasheet suggests 0x1e.
         */
        i2c_set_ccr(I2C1, 0x1e);

	/*
         * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
         * 400KHz => 300ns and 100KHz => 1000ns; 300ns/28ns = 10;
         * incremented by 1 -> 11.
         */
        i2c_set_trise(I2C1, 0x0b);

	/*
         * This is our slave address - needed only if we want to receive from
         * other masters.
         */
        i2c_set_own_7bit_slave_address(I2C1, I2C1_SLAVE_ADDR);

	/* Reenable the peripheral. */
        i2c_peripheral_enable(I2C1);
}

void i2c1_write_byte(u8 addr, u8 data)
{
	u32 reg32;

	/* Send START condition. */
        i2c_send_start(I2C1);

	/* Waiting for START is send and switched to master mode. */
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {
		__asm__("nop");
	}

	/* Send destination address. */
        i2c_send_7bit_address(I2C1, addr, I2C_WRITE);

	/* Waiting for address is transferred. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR)){
		__asm__("nop");
	}

	/* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

	/* Write data byte */
        i2c_send_data(I2C1, data);
        while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF|I2C_SR1_TxE))) {
		__asm__("nop");
	}
}

void i2c1_write_data(u8 addr, u8 *data, int size)
{
	u32 reg32;
	int i;

	/* Send START condition. */
        i2c_send_start(I2C1);

	/* Waiting for START is send and switched to master mode. */
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {
		__asm__("nop");
	}

	/* Send destination address. */
        i2c_send_7bit_address(I2C1, addr, I2C_WRITE);

	/* Waiting for address is transferred. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR)){
		__asm__("nop");
	}

	/* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

	/* Write data */
	for (i = 0; i < size; i++) {
		i2c_send_data(I2C1, data[i]);
		while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF|I2C_SR1_TxE))) {
			__asm__("nop");
		}
	}
}

void i2c1_ack_poll(u8 addr)
{
	int i;
	int done;

	done = 0;

	while (done != 1) {
		/* Send START condition. */
		i2c_send_start(I2C1);

		/* Waiting for START is send and switched to master mode. */
		while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
				& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {
			__asm__("nop");
		}

		/* Send destination address. */
		i2c_send_7bit_address(I2C1, addr, I2C_WRITE);

		/* Waiting for address is transferred. */
		i = 1000;
		while ((!(I2C_SR1(I2C1) & I2C_SR1_ADDR)) && (i != 0)){
			i--;
		}

		if (i > 0) {
			done = 1;
		}
	}
}

u8 i2c1_read_byte(u8 addr)
{
	u32 reg32;

	/* Sending another START condition. */
        i2c_send_start(I2C1);

	/* Waiting for START is send and switched to master mode. */
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {
		__asm__("nop");
	}

	/* Say to what address we want to talk to. */
        i2c_send_7bit_address(I2C1, addr, I2C_READ);

	/* Waiting for address is transferred. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR)) {
		__asm__("nop");
	}

	/* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

	/* Cleaning I2C_SR1_ACK. */
        I2C_CR1(I2C1) &= ~ I2C_CR1_ACK;

	/* Now the slave should begin to send us the first byte. Await BTF. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_BTF)) {
		__asm__("nop");
	}

	return (u8)(I2C_DR(I2C1));
}

void i2c1_end_transaction(void)
{

	/* Send stop condition. */
	I2C_CR1(I2C1) |= I2C_CR1_STOP;
}
