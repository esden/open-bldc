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

/**
 * @file   mcu.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Mar 18 2011
 *
 * @brief  mcu "driver" implementation.
 *
 * Implements functions for initializing global mcu specific features like the rcc.
 */

#include "config.h"

#include <libopencm3/stm32/rcc.h>

#include "types.h"
#include "driver/mcu.h"

/* 
 * Expanding clock configuration settings from the board settings into the
 * apropriate function name. Yes C macro language is really really retarded and
 * needs three macros to achieve that incredible feat.
 *
 * Why can't I just write directy in the system init function something like this, or similar? >_< :
 *
 * rcc_clock_setup_in_hse_ ## BOARD__HSE ## _out_ ## BOARD__SYS_CLK ## ();
 */
#define __RCC_CLOCK_SETUP(__HSE, __SYS_CLK) rcc_clock_setup_in_hse_##__HSE##_out_##__SYS_CLK
#define _RCC_CLOCK_SETUP(_HSE, _SYS_CLK) __RCC_CLOCK_SETUP(_HSE, _SYS_CLK)
#define RCC_CLOCK_SETUP _RCC_CLOCK_SETUP(BOARD__HSE, BOARD__SYS_CLK)

/**
 * Initialize STM32 system specific subsystems.
 */
void mcu_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	RCC_CLOCK_SETUP();
}

