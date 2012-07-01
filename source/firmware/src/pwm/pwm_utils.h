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

/* Channel output on/off manipulation */
#define PWM_HI_ON TIM_CCER_CC1P
#define PWM_HI_OFF 0x00000000
#define PWM_LO_ON TIM_CCER_CC1NP
#define PWM_LO_OFF 0x00000000

#define pwm_enable(A_HI, A_LO, B_HI, B_LO, C_HI, C_LO)		\
	TIM1_CCER = (TIM1_CCER & 0xFAAA) |			\
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
#define pwm_conf_mode_pwm TIM_OCM_PWM1
#define pwm_conf_mode_npwm TIM_OCM_PWM2
#define pwm_conf_mode_off TIM_OCM_FROZEN
#define pwm_conf_mode_low TIM_OCM_FORCE_LOW
#define pwm_conf_mode_high TIM_OCM_FORCE_HIGH

#define pwm_conf(AMODE, BMODE, CMODE)				\
	TIM1_CCMR1 = (TIM1_CCMR1 & 0x8F8F) |			\
		     AMODE |					\
		     (BMODE << 8);				\
	TIM1_CCMR2 = (TIM1_CCMR2 & 0xFF8F) |			\
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

/* High PWM low off manipulators */
#define pwm_set_a_hpwm_b_off__c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_hpwm_b_off__c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_off__c_hpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_OFF)
#define pwm_set_a_off__b_off__c_hpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_OFF)
#define pwm_set_a_off__b_hpwm_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_hpwm_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_ON, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)

/* High off low PWM manipulators */
#define pwm_set_a_lpwm_b_off__c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_lpwm_b_off__c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_off__c_lpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_ON)
#define pwm_set_a_off__b_off__c_lpwm()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_ON)
#define pwm_set_a_off__b_lpwm_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF)
#define pwm_set_a_off__b_lpwm_c_off()				\
	pwm_disable_all();					\
	pwm_conf(pwm_conf_mode_off,				\
		 pwm_conf_mode_pwm,				\
		 pwm_conf_mode_off);				\
	pwm_enable(PWM_HI_OFF, PWM_LO_OFF,			\
		   PWM_HI_OFF, PWM_LO_ON,			\
		   PWM_HI_OFF, PWM_LO_OFF)

#endif /* __PWM_UTILS_H */
