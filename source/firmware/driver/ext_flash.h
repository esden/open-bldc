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

#ifndef __EXT_FLASH_H
#define __EXT_FLASH_H

#define EXT_FLASH_SIZE 128

extern u8 ext_flash_data[EXT_FLASH_SIZE];

void ext_flash_init(void);
u8 ext_flash_random_read(u8 addr);
void ext_flash_random_write(u8 addr, u8 data);

#endif /* __EXT_FLASH_H */
