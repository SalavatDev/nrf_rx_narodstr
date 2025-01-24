#ifndef ENCODER_H_
#define ENCODER_H_

 
#include "main.h"
#include "tim.h"
#include "NRF24.h"
#include "LCD_HD44780.h"


#define SIZE_RPM_VAL   ((uint8_t)6)

#define	IRQ_PIN_ENCODER								GPIO_PIN_10 
#define	EXTI_IRQ_ENCODER							EXTI15_10_IRQn
#define	ENABLE_IT_EXTI_IRQ_ENCODER		HAL_NVIC_EnableIRQ(EXTI_IRQ_ENCODER)  
#define	DISABLE_IT_EXTI_IRQ_ENCODER		HAL_NVIC_DisableIRQ(EXTI_IRQ_ENCODER) 

#define	START_IT_TIM_ENCODEER		HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL)
#define	STOP_IT_TIM_ENCODEER		HAL_TIM_Encoder_Stop_IT(&htim3, TIM_CHANNEL_ALL);
#define	CNT_TIM_ENCODER 				TIM3->CNT


enum {
	
	RPM_1 = 0xFFFF,
	RPM_15 = 0x1194,	//4500
	RPM_30 = 0x08CA,	//2250
	RPM_60 = 0x0465,	//1125
	RPM_90 = 0x02ED,  //749
	RPM_120 = 0x0232 	//562
	
};

extern const uint16_t rpm_val[];
extern uint16_t	current_rpm_val;
extern	const uint16_t rpm[];

#endif

