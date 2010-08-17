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

/**
 * @file   led_fade.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:59:14 2010
 *
 * @brief  @todo document
 *
 * @todo document
 */

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>
#include <stm32/gpio.h>

#include "types.h"
#include "driver/led.h"

#include "led_fade.h"

/**
 * Gamma correction table with gamma value of 2.5
 */
static const u16 gamma_table[] = {
	0, 0, 0, 1, 2, 4, 6, 8,
	11, 15, 20, 25, 31, 38, 46, 55,
	65, 75, 87, 99, 113, 128, 143, 160,
	178, 197, 218, 239, 262, 286, 311, 338,
	366, 395, 425, 457, 491, 526, 562, 599,
	639, 679, 722, 765, 811, 857, 906, 956,
	1007, 1061, 1116, 1172, 1231, 1291, 1352, 1416,
	1481, 1548, 1617, 1688, 1760, 1834, 1910, 1988,
	2068, 2150, 2233, 2319, 2407, 2496, 2587, 2681,
	2776, 2874, 2973, 3075, 3178, 3284, 3391, 3501,
	3613, 3727, 3843, 3961, 4082, 4204, 4329, 4456,
	4585, 4716, 4850, 4986, 5124, 5264, 5407, 5552,
	5699, 5849, 6001, 6155, 6311, 6470, 6632, 6795,
	6962, 7130, 7301, 7475, 7650, 7829, 8009, 8193,
	8379, 8567, 8758, 8951, 9147, 9345, 9546, 9750,
	9956, 10165, 10376, 10590, 10806, 11025, 11247, 11472,
	11699, 11929, 12161, 12397, 12634, 12875, 13119, 13365,
	13614, 13865, 14120, 14377, 14637, 14899, 15165, 15433,
	15705, 15979, 16256, 16535, 16818, 17104, 17392, 17683,
	17978, 18275, 18575, 18878, 19184, 19493, 19805, 20119,
	20437, 20758, 21082, 21409, 21739, 22072, 22407, 22746,
	23089, 23434, 23782, 24133, 24487, 24845, 25206, 25569,
	25936, 26306, 26679, 27055, 27435, 27818, 28203, 28592,
	28985, 29380, 29779, 30181, 30586, 30994, 31406, 31820,
	32239, 32660, 33085, 33513, 33944, 34379, 34817, 35258,
	35702, 36150, 36602, 37056, 37514, 37976, 38441, 38909,
	39380, 39856, 40334, 40816, 41301, 41790, 42282, 42778,
	43277, 43780, 44286, 44795, 45308, 45825, 46345, 46869,
	47396, 47927, 48461, 48999, 49540, 50085, 50634, 51186,
	51742, 52301, 52864, 53431, 54001, 54575, 55153, 55734,
	56318, 56907, 57499, 58095, 58695, 59298, 59905, 60515,
	61130, 61748, 62370, 62995, 63624, 64258, 64894, 65535
};

/**
 * Color channel state structure
 */
struct color_channel {
	u16 curr_end_reload;	/**< @todo document */
	u32 curr_val;		/**< @todo document */
	u32 next_val;		/**< @todo document */
	u16 reloads;		/**< @todo document */
};

/**
 * Color channel instances
 */
struct color_channels {
	struct color_channel o; /**< @todo document */
	struct color_channel r; /**< @todo document */
	struct color_channel g; /**< @todo document */
	struct color_channel b; /**< @todo document */
} color_channels;

/** @todo document */
u16 delay;
/** @todo document */
u16 cnt;
/** @todo document */
s16 dir;

/** @todo document */
u16 rdelay;
/** @todo document */
u16 rcnt;
/** @todo document */
s16 rdir;

/**
 * @todo document
 *
 */
void led_fade_init(void)
{
	NVIC_InitTypeDef nvic;
	TIM_TimeBaseInitTypeDef tim_base;
	TIM_OCInitTypeDef tim_oc;

	color_channels.o.curr_end_reload = 0;
	color_channels.o.curr_val = 0;
	color_channels.o.next_val = 0;
	color_channels.o.reloads = 0;

	delay = 0;
	cnt = 5;
	dir = 1;

	color_channels.r.curr_end_reload = 0;
	color_channels.r.curr_val = 0;
	color_channels.r.next_val = 0;
	color_channels.r.reloads = 0;

	rdelay = 0;
	rcnt = 5;
	rdir = 1;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Enable the TIM3 gloabal interrupt */
	nvic.NVIC_IRQChannel = TIM3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic);

	/* GPIOA: TIM3 channel 2 as alternate function push-pull */
	//gpio.GPIO_Pin   = GPIO_Pin_7;
	//gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
	//gpio.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOA, &gpio);

	/* GPIOB: TIM33 channel 3 as alternate function push-pull */
	//gpio.GPIO_Pin   = GPIO_Pin_0;
	//GPIO_Init(GPIOB, &gpio);

	/* TIM3 time base configuration */
	tim_base.TIM_Period = 0xFFFF;
	tim_base.TIM_Prescaler = 0;
	tim_base.TIM_ClockDivision = 0;
	tim_base.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &tim_base);

	/* TIM3 Output Compare Timing Mode configuration: Channel1 */
	tim_oc.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = 0;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &tim_oc);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = 0;

	TIM_OC2Init(TIM3, &tim_oc);

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = 0;

	TIM_OC3Init(TIM3, &tim_oc);

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = 0;

	TIM_OC4Init(TIM3, &tim_oc);

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* TIM3 IT enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 |
		     TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update, ENABLE);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_Cmd(TIM3, ENABLE);

	led_fade_set_orange(0);
	led_fade_set_red(0);
	led_fade_set_green(0);
	led_fade_set_blue(0);
}

/**
 * @todo document
 *
 * @param val @todo document
 */
void led_fade_set_orange(u8 val)
{
	TIM_SetCompare1(TIM3, gamma_table[val]);
	color_channels.o.curr_val = gamma_table[val];
}

/**
 * @todo document
 *
 * @param val @todo document
 */
void led_fade_set_red(u8 val)
{
	TIM_SetCompare2(TIM3, gamma_table[val]);
	color_channels.r.curr_val = gamma_table[val];
}

/**
 * @todo document
 *
 * @param val @todo document
 */
void led_fade_set_green(u8 val)
{
	TIM_SetCompare3(TIM3, gamma_table[val]);
	color_channels.g.curr_val = gamma_table[val];
}

/**
 * @todo document
 *
 * @param val @todo document
 */
void led_fade_set_blue(u8 val)
{
	TIM_SetCompare4(TIM3, gamma_table[val]);
	color_channels.b.curr_val = gamma_table[val];
}

/**
 * @todo document
 *
 */
void tim3_irq_handler(void)
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		if (color_channels.o.curr_val) {
			LED_ORANGE_ON();
		}
		if (color_channels.r.curr_val) {
			LED_RED_ON();
		}
		if (color_channels.g.curr_val) {
			LED_GREEN_ON();
		}
		if (color_channels.b.curr_val) {
			LED_BLUE_ON();
		}
	}

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

		LED_ORANGE_OFF();
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

		LED_RED_OFF();
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

		LED_GREEN_OFF();
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

		LED_BLUE_OFF();
	}
}
