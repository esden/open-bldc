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

#ifndef __GPROT_H
#define __GPROT_H

void gprot_init();
u8   gprot_handle(u8 byte);
s16  gprot_get_byte();
void gprot_conf_reg(u8 addr, u16 *value);
void gprot_toggle_cont_reg(u8 addr);
void gprot_poke_cont_reg(u8 addr);
void gprot_poke_reg(u8 addr);
u8 gprot_get_next_addr(void);

#endif /* __GPROT_H */
