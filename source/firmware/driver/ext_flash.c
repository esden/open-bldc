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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

u8 ext_flash_data[EXT_FLASH_SIZE];

/**
 * Initialize external flash driver.
 */
void ext_flash_init(void)
{
	int i, j;

	/* Initializing i2c1 */
	i2c1_init();

	/* Initializing the ram mirror of the flash content. */
	for (i = 0; i < EXT_FLASH_SIZE; i++) {
		ext_flash_data[i] = 0;
	}

	/* Writing some sequential data to the flash. Just testing. */
	for (j = 0; j < EXT_FLASH_SIZE; j++) {
		ext_flash_random_write(j, j);

		ON(LED_RED);
		for (i = 0; i < 15000; i++) {
			__asm__("nop");
		}
		OFF(LED_RED);
		for (i = 0; i < 15000; i++) {
			__asm__("nop");
		}
	}

	/* Reading out the flash data into the ram mirror. */
	for (i = 0; i < EXT_FLASH_SIZE; i++) {
		ext_flash_data[i] = ext_flash_random_read(i);
	}

}

void ext_flash_random_write(u8 addr, u8 data)
{
	u8 i2c_data[2];

	i2c_data[0] = addr;
	i2c_data[1] = data;

	/* Write data byte to address addr */
	i2c1_write_data(0x50, i2c_data, 2);

	/* End i2c1 transaction */
	i2c1_end_transaction();
}

u8 ext_flash_random_read(u8 addr)
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
