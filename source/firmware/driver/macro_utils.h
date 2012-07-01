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

#ifndef __MACRO_UTILS_H
#define __MACRO_UTILS_H

#define OFF(PORT_PIN)					\
	gpio_set(PORT_PIN##_PORT, PORT_PIN##_PIN)

#define ON(PORT_PIN)					\
	gpio_clear(PORT_PIN##_PORT, PORT_PIN##_PIN)

#define TOGGLE(PORT_PIN) {						\
		if (gpio_get(PORT_PIN##_PORT, PORT_PIN##_PIN) != 0) {	\
			ON(PORT_PIN);					\
		}else{							\
			OFF(PORT_PIN);					\
		}							\
	}

#endif /* __MACRO_UTILS_H */
