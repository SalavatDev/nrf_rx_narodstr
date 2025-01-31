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
 ******************************************************************************
 * @file    stepper_motor.h
 * @brief   pulse generation module for stepper motor.
 * @author  Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
 * 	This module contains functions for controlling the smooth acceleration of the 
 * 	stepper motor and interrupt handlers of the PWM generator, as well as state 
 * 	variables of the module.
 * 
 * 
 * Need to perform :
 *     TODO 
 * 
 * Log: 
 *     Version 1.0.0.0: Initial version.  
 *         
 * 
 * Known Bugs:
 *     TODO BUG 
 * 
 * Fixed Bugs:
 *     BUG FIXED dd.mm.yyyy 431 Description
 *
 ******************************************************************************/

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_




/************************************
 * INCLUDES							
 ************************************/
#include "main.h"
#include "NRF24.h"
#include "LCD_HD44780.h"
#include <math.h>
#include "encoder.h"
#include <stdio.h>



#ifdef __cplusplus
extern "C" {
#endif



/************************************
 * MACROS AND DEFINES
 ************************************/
#define	RIGHT_DIR	1

#define DELAY_TIM_ANTI_DREBEZG				{ DelayMicro(65000); DelayMicro(65000);  DelayMicro(65000); }  //65ms*3=195 ms

#define	START_TIM_DELAY_CHANGE_STEP		HAL_TIM_Base_Start(&htim4)
#define	CNT_TIM_DELAY_MOTOR_STEP			TIM4->CNT

#define	START_IT_TIM_STEPPER					HAL_TIM_Base_Start_IT(&htim1)
#define	STOP_IT_TIM_STEPPER						HAL_TIM_Base_Stop_IT(&htim1)


 

/************************************
 * TYPEDEFS
 ************************************/

// the data type for storing the current state of 
//the angle and the synchronization flag
typedef struct {

	float angle;  
	uint8_t synchr ;
	
}StepMotorAngle;

//type for indicating whether synchronization is on or off
enum {
	
	SYNCHR_ON = 1,
	SYNCHR_OFF = 0
	
};


//type of quarter definition
enum {
	
	FIRST_SECOND_QUARTER = 0,
	THIRD_FOURTH_QUARTER = 1,
	ANOTHER_QUARTER = 2
};




/************************************
 * EXPORTED VARIABLES
 ************************************/
extern StepMotorAngle pulse_angle;
extern uint8_t show_delta_angle; 
extern uint8_t quarter_reciver;
extern uint8_t change_period_step;
extern uint16_t current_rpm_val;





/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
  * @brief stepper motor smooth acceleration function
  * @param new_rpm_val new stepper motor speed value
	* @param current_rpm_val pointer to current stepper motor speed value
  * @retval None
  */
void motor_step_period_change(uint16_t new_rpm_val, uint16_t *current_rpm_val);






#ifdef __cplusplus
}
#endif


#endif


