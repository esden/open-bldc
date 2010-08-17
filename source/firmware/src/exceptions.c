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
 * @file   exceptions.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 02:01:35 2010
 *
 * @brief  @todo document
 *
 * @todo document
 */


#include "exceptions.h"

/**
 * @todo document
 *
 */
void nmi_exception(void)
{
}

/**
 * @todo document
 *
 */
void hard_fault_exception(void)
{
	while (1) ;
}

/**
 * @todo document
 *
 */
void mem_manage_exception(void)
{
	while (1) ;
}

/**
 * @todo document
 *
 */
void bus_fault_exception(void)
{
	while (1) ;
}

/**
 * @todo document
 *
 */
void usage_fault_exception(void)
{
	while (1) ;
}
