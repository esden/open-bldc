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

#ifndef __DEBUG_PINS_H
#define __DEBUG_PINS_H

#include "config.h"

#define DP_ENC_A_PORT GPIOA
#define DP_ENC_A_BIT (0x00000001 << 6)

#define DP_ENC_B_PORT GPIOA
#define DP_ENC_B_BIT (0x00000001 << 7)

#define DP_EXT_SCL_PORT GPIOB
#define DP_EXT_SCL_BIT (0x00000001 << 8)

#define DP_EXT_SDA_PORT GPIOB
#define DP_EXT_SDA_BIT (0x00000001 << 9)

#ifdef DP_USE_ENCODER
#define DP_ENC_A_OFF() DP_ENC_A_PORT->BSRR |= DP_ENC_A_BIT
#define DP_ENC_A_ON() DP_ENC_A_PORT->BRR |= DP_ENC_A_BIT
#define DP_ENC_A_TOGGLE() {						\
		if ((DP_ENC_A_PORT->IDR & DP_ENC_A_BIT) != 0) {		\
			DP_ENC_A_ON();					\
		}else{							\
			DP_ENC_A_OFF();					\
		}							\
	}

#define DP_ENC_B_OFF() DP_ENC_B_PORT->BSRR |= DP_ENC_B_BIT
#define DP_ENC_B_ON() DP_ENC_B_PORT->BRR |= DP_ENC_B_BIT
#define DP_ENC_B_TOGGLE() {						\
		if ((DP_ENC_B_PORT->IDR & DP_ENC_B_BIT) != 0) {		\
			DP_ENC_B_ON();					\
		}else{							\
			DP_ENC_B_OFF();					\
		}							\
	}
#else
#define DP_ENC_A_OFF() ((void)0)
#define DP_ENC_A_ON() ((void)0)
#define DP_ENC_A_TOGGLE() ((void)0)

#define DP_ENC_B_OFF() ((void)0)
#define DP_ENC_B_ON() ((void)0)
#define DP_ENC_B_TOGGLE() ((void)0)
#endif

#ifdef DP_USE_EXT_I2C
#define DP_EXT_SCL_OFF() DP_EXT_SCL_PORT->BSRR |= DP_EXT_SCL_BIT
#define DP_EXT_SCL_ON() DP_EXT_SCL_PORT->BRR |= DP_EXT_SCL_BIT
#define DP_EXT_SCL_TOGGLE() {						\
		if ((DP_EXT_SCL_PORT->IDR & DP_EXT_SCL_BIT) != 0) {	\
			DP_EXT_SCL_ON();				\
		}else{							\
			DP_EXT_SCL_OFF();				\
		}							\
	}

#define DP_EXT_SDA_OFF() DP_EXT_SDA_PORT->BSRR |= DP_EXT_SDA_BIT
#define DP_EXT_SDA_ON() DP_EXT_SDA_PORT->BRR |= DP_EXT_SDA_BIT
#define DP_EXT_SDA_TOGGLE() {						\
		if ((DP_EXT_SDA_PORT->IDR & DP_EXT_SDA_BIT) != 0) {	\
			DP_EXT_SDA_ON();				\
		}else{							\
			DP_EXT_SDA_OFF();				\
		}							\
	}
#else
#define DP_EXT_SCL_OFF() ((void)0)
#define DP_EXT_SCL_ON() ((void)0)
#define DP_EXT_SCL_TOGGLE() ((void)0)

#define DP_EXT_SDA_OFF() ((void)0)
#define DP_EXT_SDA_ON() ((void)0)
#define DP_EXT_SDA_TOGGLE() ((void)0)
#endif

void debug_pins_init(void);

#endif /* __DP_EXT_PINS_H */
