#ifndef MODULE_CONFIG_HPP__
#define MODULE_CONFIG_HPP__

/* Module: GPROT Flag: ADC_COMM */
#define GPROT__FLAG_ADC_COMM 2

/* Module: GPROT Flag: ALL_HI */
#define GPROT__FLAG_ALL_HI 4

/* Module: GPROT Flag: ALL_LO */
#define GPROT__FLAG_ALL_LO 3

/* Module: GPROT Flag: COMM_TIM */
#define GPROT__FLAG_COMM_TIM 1

/* Module: GPROT Flag: PWM_COMM */
#define GPROT__FLAG_PWM_COMM 0


/* Module: GPROT Register group: ADC */
#define GPROT__ADC_GLOBAL_CURRENT_REG_ADDR 8
#define GPROT__ADC_PHASE_VOLTAGE_REG_ADDR 9
#define GPROT__ADC_ZERO_VALUE_REG_ADDR 4

/* Module: GPROT Register group: CPU Load */
#define GPROT__CPU_LOAD 11
#define GPROT__CPU_LOAD_MAX 12
#define GPROT__CPU_LOAD_MIN 13

/* Module: GPROT Register group: Commutation_Timer */
#define GPROT__COMM_TIM_DIRECT_CUTOFF_REG_ADDR 6
#define GPROT__COMM_TIM_FREQ_REG_ADDR 3
#define GPROT__COMM_TIM_IIR_POLE_REG_ADDR 7
#define GPROT__COMM_TIM_SPARK_ADVANCE_REG_ADDR 5

/* Module: GPROT Register group: Flags */
#define GPROT__FLAGS_REG_ADDR 0

/* Module: GPROT Register group: Other */
#define GPROT__NEW_CYCLE_TIME 10

/* Module: GPROT Register group: PWM */
#define GPROT__PWM_OFFSET_REG_ADDR 1
#define GPROT__PWM_VAL_REG_ADDR 2


/* Module: TARGET Define: ADC_BIT_RES */
#define TARGET__ADC_BIT_RES 12

/* Module: TARGET Define: ADC_TIM */
#define TARGET__ADC_TIM TIM4

/* Module: TARGET Define: IMPORT_CONFIG */
#define TARGET__IMPORT_CONFIG ./test/target_defs.yaml

/* Module: TARGET Define: NUM_ADC */
#define TARGET__NUM_ADC 10

/* Module: TARGET Define: USE_ADC1 */
#define TARGET__USE_ADC1
/* Module: TARGET Define: USE_ADC2 */
// #define TARGET__USE_ADC2
/* Module: TARGET Define: USE_ADC3 */
#define TARGET__USE_ADC3

#endif /* MODULE_CONFIG_HPP__ */
