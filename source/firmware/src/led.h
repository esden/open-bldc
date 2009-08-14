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

#define LED_ORANGE_OFF() GPIOB->BSRR |= 0x00000004
#define LED_ORANGE_ON() GPIOB->BRR |= 0x00000004
#define LED_ORANGE_TOGGLE() {                   \
        if(GPIOB->IDR & 0x00000004){            \
            LED_ORANGE_ON();                    \
        }else{                                  \
            LED_ORANGE_OFF();                   \
        }                                       \
    }

#define LED_RED_OFF() GPIOB->BSRR |= 0x00001000
#define LED_RED_ON() GPIOB->BRR |= 0x00001000
#define LED_RED_TOGGLE() {                      \
        if(GPIOB->IDR & 0x00001000){            \
            LED_ORANGE_ON();                    \
        }else{                                  \
            LED_ORANGE_OFF();                   \
        }                                       \
    }

#define LED_GREEN_OFF() GPIOB->BSRR |= 0x00000001
#define LED_GREEN_ON() GPIOB->BRR |= 0x00000001
#define LED_GREEN_TOGGLE() {                    \
    if(GPIOB->IDR & 0x00000001){                \
        LED_ORANGE_ON();                        \
    }else{                                      \
        LED_ORANGE_OFF();                       \
    }                                           \
}

#define LED_BLUE_OFF() GPIOA->BSRR |= 0x00000080
#define LED_BLUE_ON() GPIOA->BRR |= 0x00000080
#define LED_BLUE_TOGGLE() {                     \
        if(GPIOB->IDR & 0x00000008){            \
            LED_ORANGE_ON();                    \
        }else{                                  \
            LED_ORANGE_OFF();                   \
        }                                       \
    }

void led_init(void);

#endif /* __LED_H */
