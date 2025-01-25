/*******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Technology OFS.
 * All rights reserved.
 *
 * The information contained herein is confidential
 * property of Technology OFS.
 * The use, copying, transfer or disclosure of such
* information is prohibited except by express written
 * agreement with Technology OFS.
 * ******************************************************************************
 * @file    encoder.h
 * @brief   module for encoder operation
 * @author  Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
 *   Module Description:
 * 	This module is designed to control an incremental encoder, contains a callback handler 
 *  function for implementing rotation changes and button press processing, and also stores the 
 * 	current state.
 *
 * Need to perform :
 *     TODO 
 * 
 * Log: 
 *     Version 1.0.0.0:  
 *         
 * 
 * Known Bugs:
 *     TODO BUG 
 * 
 * Fixed Bugs:
 *     BUG FIXED dd.mm.yyyy 431 Description
 * 
 ******************************************************************************/

#ifndef ENCODER_H_
#define ENCODER_H_

/************************************
 * INCLUDES							
 ************************************/
#include "main.h"
#include "tim.h"
#include "NRF24.h"
#include "LCD_HD44780.h"
#include "stepper_motor.h"
#include "app_lev_stend.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * MACROS AND DEFINES
 ************************************/
#define SIZE_RPM_VAL   ((uint8_t)7)
#define	IRQ_PIN_ENCODER								GPIO_PIN_10 
#define	EXTI_IRQ_ENCODER							EXTI15_10_IRQn
#define	ENABLE_IT_EXTI_IRQ_ENCODER		HAL_NVIC_EnableIRQ(EXTI_IRQ_ENCODER)  
#define	DISABLE_IT_EXTI_IRQ_ENCODER		HAL_NVIC_DisableIRQ(EXTI_IRQ_ENCODER) 
#define	START_IT_TIM_ENCODEER		HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL)
#define	STOP_IT_TIM_ENCODEER		HAL_TIM_Encoder_Stop_IT(&htim3, TIM_CHANNEL_ALL);
#define	CNT_TIM_ENCODER 				TIM3->CNT


/************************************
 * TYPEDEFS
 ************************************/
enum {
	
	RPM_0 = 0x0000,
	RPM_1 = 0xFFFF,
	RPM_15 = 0x1194,	//4500
	RPM_30 = 0x08CA,	//2250
	RPM_60 = 0x0465,	//1125
	RPM_90 = 0x02ED,  //749
	RPM_120 = 0x0232 	//562
	
};



/************************************
 * EXPORTED VARIABLES
 ************************************/
extern const uint16_t rpm_val[];
extern uint16_t	current_rpm_val;
extern	const uint16_t rpm[];



/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


void PressButtonCallbackHandler(void);



#ifdef __cplusplus
}
#endif

#endif

