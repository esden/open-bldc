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

#include "driver/ext_flash.h"

#include "driver/i2c1.h"
#include "driver/led.h"

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/i2c.h>

u8 ext_flash_data[EXT_FLASH_SIZE];

/*
 * Private function definitions.
 */
void ext_flash_hw_random_write(u8 addr, u8 data);
u8 ext_flash_hw_random_read(u8 addr);

/*
 * This is an implementation of the abstract driver for the
 * 24AA01H/24LC01BH 1kBit I2C flash chip with half array write
 * protect.
 */

/**
 * Initialize external flash driver.
 */
void ext_flash_init(void)
{
	int i;

	/* Initializing i2c1 */
	i2c1_init();

	/* Initializing the ram mirror of the flash content. */
	for (i = 0; i < EXT_FLASH_SIZE; i++) {
		ext_flash_data[i] = 0;
	}


	/* Read out the flash content into the mirror as we most
	 * likely want that anyways. :)
	 */
	ext_flash_read();
}

/**
 * Store the content of the flash ram mirror to the real flash.
 *
 * @return Returns 0 when the new content of the flash matches the
 * mirror, 1 if there is a mismatch in the potentially write protected
 * part of flash and 2 when the mismatch is in the not write protected
 * part of memory.
 */
int ext_flash_store(void)
{
	int addr, delay;

	/* Write the content of the ram mirror to flash */
	for (addr = 0; addr < EXT_FLASH_SIZE; addr++) {
		ext_flash_hw_random_write(addr, ext_flash_data[addr]);

		/*
		 * Waiting a little for the flash chip to finish
		 * writing the byte and while we do that we indicate
		 * that we are doing something.
		 *
		 * @todo I think we should use a delay subsystem based
		 * on sys tick or even better implement the ack
		 * polling system of the flash chip!!! (esden)
		 */
		ON(LED_RED);
		for (delay = 0; delay < 15000; delay++) {
			__asm__("nop");
		}
		OFF(LED_RED);
		for (delay = 0; delay < 15000; delay++) {
			__asm__("nop");
		}
	}

	/* Read flash content to check if we succeeded to write */
	for (addr = 0; addr < EXT_FLASH_SIZE; addr++) {
		if (ext_flash_data[addr] != ext_flash_hw_random_read(addr)) {
			if (addr < 0x40) {
				/*
				 * Found a mismatch in the non
				 * protected part of memory!!!
				 */
				return 2;
			} else {
				/*
				 * Found a mismatch in the potentially
				 * write protected memory.
				 */
				return 1;
			}
		}
	}

	return 0;
}

/**
 * Read the flash memory content from the hardware flash to the memory
 * mirror.
 */
int ext_flash_read(void)
{
	int i;

	/* Reading out the flash data into the ram mirror. */
	for (i = 0; i < EXT_FLASH_SIZE; i++) {
		ext_flash_data[i] = ext_flash_hw_random_read(i);
	}

	return i;
}

/**
 * Get a byte from the flash memory mirror.
 *
 * @return -1 if the supplied address is out of bounds otherwise the
 * value of the byte.
 */
s16 ext_flash_get_byte(u8 addr)
{
	if (addr < EXT_FLASH_SIZE) {
		return ext_flash_data[addr];
	}

	/* address was invalid!!! */
	return -1;
}

/**
 * Set a byte in the flash memory mirror.
 *
 * @return 1 if the supplied address is out of bounds.
 */
int ext_flash_set_byte(u8 addr, u8 data)
{

	if (addr < EXT_FLASH_SIZE) {
		ext_flash_data[addr] = data;
		return 0;
	}

	return 1;
}

/**
 * Private function, writing to a random address in the real flash.
 */
void ext_flash_hw_random_write(u8 addr, u8 data)
{
	u8 i2c_data[2];

	i2c_data[0] = addr;
	i2c_data[1] = data;

	/* Write data byte to address addr */
	i2c1_write_data(0x50, i2c_data, 2);

	/* End i2c1 transaction */
	i2c1_end_transaction();
}

/**
 * Private function, reading from a random address in the real flash.
 */
u8 ext_flash_hw_random_read(u8 addr)
{
	u8 ret;

	/* Setting up the startaddress from where we want to read a byte. */
	i2c1_write_byte(0x50, addr);

	/* Reading the data out of the register. */
	ret = i2c1_read_byte(0x50);

	/* End i2c1 transaction */
	i2c1_end_transaction();

	return ret;
}
