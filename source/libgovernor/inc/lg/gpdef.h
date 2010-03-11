/*
 * libgovernor - Open-BLDC configuration and debug protocol library
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

/* governor protocol definitions */

#ifndef GPDEF_H
#define GPDEF_H

typedef void (*gp_simple_hook_t)();
typedef void (*gp_with_addr_hook_t)(u8 addr);

#define GP_MODE_PEEK 0
#define GP_MODE_CONT (1 << 6)
#define GP_MODE_READ 0
#define GP_MODE_WRITE (1 << 7)

#endif /* GPDEF_H */
