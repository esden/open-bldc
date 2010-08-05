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

#ifndef __LED_H
#define __LED_H

#define LED_ORANGE_PORT GPIOB
#define LED_ORANGE_BIT (0x00000001 << 2)

#define LED_RED_PORT GPIOB
#define LED_RED_BIT    (0x00000001 << 12)

#define LED_GREEN_PORT GPIOC
#define LED_GREEN_BIT  (0x00000001 << 14)

#define LED_BLUE_PORT GPIOC
#define LED_BLUE_BIT   (0x00000001 << 13)

#define LED_ORANGE_OFF() LED_ORANGE_PORT->BSRR |= LED_ORANGE_BIT
#define LED_ORANGE_ON() LED_ORANGE_PORT->BRR |= LED_ORANGE_BIT
#define LED_ORANGE_TOGGLE() {				      \
		if(LED_ORANGE_PORT->IDR & LED_ORANGE_BIT){    \
			LED_ORANGE_ON();		      \
		}else{					      \
			LED_ORANGE_OFF();		      \
		}					      \
	}

#define LED_RED_OFF() LED_RED_PORT->BSRR |= LED_RED_BIT
#define LED_RED_ON() LED_RED_PORT->BRR |= LED_RED_BIT
#define LED_RED_TOGGLE() {				\
		if(LED_RED_PORT->IDR & LED_RED_BIT){	\
			LED_RED_ON();			\
		}else{                                  \
			LED_RED_OFF();			\
		}                                       \
	}

#define LED_GREEN_OFF() LED_GREEN_PORT->BSRR |= LED_GREEN_BIT
#define LED_GREEN_ON() LED_GREEN_PORT->BRR |= LED_GREEN_BIT
#define LED_GREEN_TOGGLE() {					\
		if(LED_GREEN_PORT->IDR & LED_GREEN_BIT){	\
			LED_GREEN_ON();				\
		}else{						\
			LED_GREEN_OFF();			\
		}						\
	}

#define LED_BLUE_OFF() LED_BLUE_PORT->BSRR |= LED_BLUE_BIT
#define LED_BLUE_ON() LED_BLUE_PORT->BRR |= LED_BLUE_BIT
#define LED_BLUE_TOGGLE() {				\
		if(LED_BLUE_PORT->IDR & LED_BLUE_BIT){	\
			LED_BLUE_ON();			\
		}else{                                  \
			LED_BLUE_OFF();			\
		}                                       \
	}

void led_init(void);

#endif /* __LED_H */
