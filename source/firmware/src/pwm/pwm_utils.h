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

#ifndef __PWM_UTILS_H
#define __PWM_UTILS_H

#define PWM_PHASE_A TIM_Channel_1
#define PWM_PHASE_B TIM_Channel_2
#define PWM_PHASE_C TIM_Channel_3


/* Channel output on/off manipulation */
#define PWM_HI_ON TIM_CCx_Enable
#define PWM_HI_OFF TIM_CCx_Disable
#define PWM_LO_ON TIM_CCxN_Enable
#define PWM_LO_OFF TIM_CCxN_Disable

#define pwm_enable(A_HI, A_LO, B_HI, B_LO, C_HI, C_LO)		\
	TIM1->CCER = (TIM1->CCER & 0xF000) |			\
		     ((A_HI | A_LO) << 0) |			\
		     ((B_HI | B_LO) << 4) |			\
		     ((C_HI | C_LO) << 8)

#define pwm_enable_all()					\
	pwm_enable(PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_ON, PWM_LO_ON)

#define pwm_disable_all()					\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)

/* Configure channels */
#define pwm_conf_mode_pwm TIM_OCMode_PWM1
#define pwm_conf_mode_npwm TIM_OCMode_PWM2
#define pwm_conf_mode_off TIM_OCMode_Timing
#define pwm_conf_mode_low TIM_ForcedAction_InActive
#define pwm_conf_mode_high TIM_ForcedAction_Active

#define pwm_conf(AMODE, BMODE, CMODE)				\
	TIM1->CCMR1 = (TIM1->CCMR1 & 0x8F8F) |			\
		      AMODE |					\
		      (BMODE << 8);				\
	TIM1->CCMR2 = (TIM1->CCMR2 & 0xFF8F) |			\
		      CMODE

#define pwm_conf_all_active()					\
	pwm_conf(pwm_conf_mode_high,				\
		 pwm_conf_mode_high,				\
		 pwm_conf_mode_high)
#define pwm_conf_all_inactive()					\
	pwm_conf(pwm_conf_mode_low,				\
		 pwm_conf_mode_low,				\
		 pwm_conf_mode_low)

/* Channel and pwm on/off manipulation */
#define pwm_set_all_off()					\
	pwm_disable_all();

#define pwm_set_all_high()					\
	pwm_disable_all();					\
	pwm_conf_all_active();					\
	pwm_enable_all()

#define pwm_set_all_low()					\
	pwm_disable_all();					\
	pwm_conf_all_inactive();				\
	pwm_enable_all()

/* High PWM low on manipulators */
#define pwm_set_a_hpwm_b_off__c_low()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_low);				\
	pwm_enable(PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_ON)
#define pwm_set_a_hpwm_b_low__c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_low,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_low__c_hpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_low,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_ON, PWM_LO_OFF)
#define pwm_set_a_low__b_off__c_hpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_low,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_OFF)
#define pwm_set_a_low__b_hpwm_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_low,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_hpwm_c_low()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_low);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_ON)

/* High on low PWM manipulators */
#define pwm_set_a_lpwm_b_off__c_high()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_high);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_ON)
#define pwm_set_a_lpwm_b_high_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_high,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_high_c_lpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_high,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_ON)
#define pwm_set_a_high_b_off__c_lpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_high,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_ON)
#define pwm_set_a_high_b_lpwm_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_high,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_ON, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_lpwm_c_high()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_high);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_ON, PWM_LO_ON)

#endif /* __PWM_UTILS_H */
