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

#ifndef __I2C1_H
#define __I2C1_H

void i2c1_init(void);
void i2c1_write_byte(u8 addr, u8 data);
u8 i2c1_read_byte(u8 addr);
void i2c1_write_data(u8 addr, u8 *data, int size);
void i2c1_end_transaction(void);
void i2c1_ack_poll(u8 addr);

#endif /* __I2C1_H */
