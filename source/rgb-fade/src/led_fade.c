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

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>
#include <stm32/gpio.h>

#include "led_fade.h"

#include "led.h"
#include "types.h"

static const u32 brightness_table[] = {
    2,	        8,	19,	34,	52,	75,	102,	134,
    169,	208,	252,	299,	351,	406,	466,	530,
    598,	669,	745,	825,	909,	998,	1090,	1186,
    1286,	1390,	1499,	1611,	1728,	1848,	1972,	2101,
    2233,	2370,	2511,	2655,	2804,	2956,	3113,	3274,
    3438,	3607,	3780,	3957,	4137,	4322,	4511,	4704,
    4900,	5101,	5306,	5515,	5728,	5944,	6165,	6390,
    6619,	6852,	7089,	7329,	7574,	7823,	8076,	8333,
    8593,	8858,	9127,	9400,	9676,	9957,	10242,	10531,
    10823,	11120,	11421,	11725,	12034,	12347,	12663,	12984,
    13309,	13637,	13970,	14306,	14647,	14991,	15340,	15692,
    16049,	16409,	16774,	17142,	17514,	17891,	18271,	18655,
    19043,	19436,	19832,	20232,	20636,	21044,	21456,	21872,
    22292,	22716,	23144,	23576,	24012,	24452,	24896,	25344,
    25795,	26251,	26711,	27175,	27642,	28114,	28589,	29069,
    29552,	30040,	30531,	31027,	31526,	32029,	32537,	33048,
    33563,	34082,	34605,	35132,	35663,	36198,	36737,	37280,
    37827,	38378,	38932,	39491,	40054,	40621,	41191,	41766,
    42344,	42927,	43513,	44103,	44698,	45296,	45898,	46504,
    47114,	47728,	48347,	48968,	49594,	50224,	50858,	51496,
    52138,	52783,	53433,	54086,	54744,	55405,	56071,	56740,
    57414,	58091,	58772,	59457,	60146,	60839,	61536,	62237,
    62942,	63651,	64364,	65080,	65801,	66526,	67254,	67987,
    68723,	69463,	70208,	70956,	71708,	72464,	73224,	73988,
    74756,	75528,	76304,	77084,	77867,	78655,	79447,	80242,
    81042,	81845,	82652,	83464,	84279,	85098,	85921,	86748,
    87579,	88414,	89253,	90096,	90942,	91793,	92648,	93506,
    94369,	95235,	96105,	96980,	97858,	98740,	99626,	100516,
    101410,	102308,	103209,	104115,	105025,	105938,	106856,	107777,
    108703,	109632,	110565,	111502,	112444,	113389,	114338,	115290,
    116247,	117208,	118173,	119141,	120114,	121090,	122071,	123055,
    124043,	125035,	126032,	127032,	128036,	129043,	130055,	131071
};

struct color_channel {
    u16 curr_end_reload;
    u32 curr_val;
    u32 next_val;
    u16 reloads;
};

struct color_channels {
    struct color_channel o;
    struct color_channel r;
    struct color_channel g;
    struct color_channel b;
} color_channels;

u16 delay;
u16 cnt;
s16 dir;

u16 rdelay;
u16 rcnt;
s16 rdir;

void led_fade_init(void){
    NVIC_InitTypeDef nvic;
    TIM_TimeBaseInitTypeDef tim_base;
    TIM_OCInitTypeDef       tim_oc;

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
    tim_base.TIM_Prescaler = 4;
    tim_base.TIM_ClockDivision = 0;
    tim_base.TIM_CounterMode = TIM_CounterMode_CenterAligned3;

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
                       TIM_IT_CC3 | TIM_IT_CC4 |
                       TIM_IT_Update, ENABLE);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}

void tim3_irq_handler(void){

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        if(!color_channels.o.curr_val){
            if(color_channels.o.curr_val & (1 << 16)){
                TIM_SetCompare1(TIM3, (u16)(0xFFFF - (color_channels.o.next_val & 0xFFFF)));
            }else{
                TIM_SetCompare1(TIM3, (u16)(color_channels.o.next_val & 0xFFFF));
            }
            if(color_channels.o.next_val) LED_ORANGE_ON();
        }else if(!(TIM3->CR1 & (1<<4))) LED_ORANGE_ON();

        if(!(TIM3->CR1 & (1<<4))){
            delay++;
            if(delay == 2){
                delay = 0;
                if(dir == 1){
                    cnt++;
                }else{
                    cnt--;
                }
                if(cnt == 250){
                    dir = -1;
                }
                if(cnt == 5){
                    dir = 1;
                }
                color_channels.o.next_val = brightness_table[cnt];
            }
        }

        if(!color_channels.r.curr_val){
            if(color_channels.r.curr_val & (1 << 16)){
                TIM_SetCompare2(TIM3, (u16)(0xFFFF - (color_channels.r.next_val & 0xFFFF)));
            }else{
                TIM_SetCompare2(TIM3, (u16)(color_channels.r.next_val & 0xFFFF));
            }
            if(color_channels.r.next_val) LED_RED_ON();
        }else if(!(TIM3->CR1 & (1<<4))) LED_RED_ON();

        if(!(TIM3->CR1 & (1<<4))){
            rdelay++;
            if(rdelay == 2){
                rdelay = 0;
                if(rdir == 1){
                    rcnt++;
                }else{
                    rcnt--;
                }
                if(rcnt == 250){
                    rdir = -1;
                }
                if(rcnt == 5){
                    rdir = 1;
                }
                color_channels.r.next_val = brightness_table[rcnt];
            }
        }

    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

        if(( (color_channels.o.curr_val & (1 << 16)) && (TIM3->CR1 & (1<<4))) ||
           (!(color_channels.o.curr_val & (1 << 16)) && !(TIM3->CR1 & (1<<4)))){
            LED_ORANGE_OFF();
            if(color_channels.o.next_val & (1 << 16)){
                TIM_SetCompare1(TIM3, (u16)(0xFFFF - (color_channels.o.next_val & 0xFFFF)));
            }else{
                TIM_SetCompare1(TIM3, (u16)(color_channels.o.next_val & 0xFFFF));
            }
            color_channels.o.curr_val = color_channels.o.next_val;
        }

    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

        if(( (color_channels.r.curr_val & (1 << 16)) && (TIM3->CR2 & (1<<4))) ||
           (!(color_channels.r.curr_val & (1 << 16)) && !(TIM3->CR2 & (1<<4)))){
            LED_RED_OFF();
            if(color_channels.r.next_val & (1 << 16)){
                TIM_SetCompare2(TIM3, (u16)(0xFFFF - (color_channels.r.next_val & 0xFFFF)));
            }else{
                TIM_SetCompare2(TIM3, (u16)(color_channels.r.next_val & 0xFFFF));
            }
            color_channels.r.curr_val = color_channels.r.next_val;
        }

    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

        LED_GREEN_OFF();
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

        LED_BLUE_OFF();
    }
}
