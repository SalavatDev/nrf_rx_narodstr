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
 * @file    app_lev_stend.c
 * @brief   module for encoder operation
 * @author  Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
 *   This module is for defining and implementing top-level functions.
 *
 * Need to perform :
 *     TODO 
 * 
 * Log: 
 *     Version 1.0.0.0:  
 *         
 * 
 * Known Bugs:
 *       
 * 
 * Fixed Bugs:
 *     BUG FIXED dd.mm.yyyy 431 Description
 * 
 ******************************************************************************/
 
#include "app_lev_stend.h"



/************************************
 * STATIC VARIABLES
 ************************************/
static uint16_t delta_angle = 0, delta_angle_max = 0;




/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
 
 /**
  * @brief Initializing the display
  * @param None
  * @retval None
  */
static void InitLcd(void){
	
	START_TIM_DELAY_NRF;	
	LCD_HD44780_init();
	HAL_Delay(10);
	start_screen();
	display_clear_text();
	main_menu();
	
}


 /**
  * @brief Initializing the radio module
  * @param None
  * @retval None
  */
static void InitRadioChanel(void){
	
	Nrf24Init();
	
	ENABLE_IT_EXTI_IRQ_NRF;
	
}


 /**
  * @brief Initializing the stepper engine
  * @param None
  * @retval None
  */
static void InitStepMotor(void){
	
	if(RIGHT_DIR)
		HAL_GPIO_WritePin(DIR_ROTATION_GPIO_Port, DIR_ROTATION_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DIR_ROTATION_GPIO_Port, DIR_ROTATION_Pin, GPIO_PIN_RESET);
	
	START_TIM_DELAY_CHANGE_STEP;

}



/************************************
 * GLOBAL FUNCTIONS
 ************************************/

 /**
  * @brief Rotation editing menu
  * @param None
  * @retval None
  */
void ShowEditMenuEcnoder(void){
	
	display_clear_text();
	goto_xy(0, 0);
	display_send_char("RPM");			 			 
	display_send_num((int)rpm_val[current_cnt_encoder], 3, 0, 1);
	
}



 /**
  * @brief  menu for displaying the angle difference between 
	*					the calculated and accepted angle of the magnetometer and 
	*					the magnetometer status bits
  * @param None
  * @retval None
  */
void ShowAngleBitStat(void){

	display_send_num((int)nrf_bits_field_rxdata.bits_fld.a9, 1, 0, 1);
	display_send_num((int)nrf_bits_field_rxdata.bits_fld.a12, 1, 2, 1);
	display_send_num((int)nrf_bits_field_rxdata.bits_fld.a13, 1, 5, 1);				
				
	if(((dt > 0.0) && (dt < 180.0) && (quarter_reciver == FIRST_SECOND_QUARTER))||((dt >= 180.0) && (dt < 360.0) && (quarter_reciver == THIRD_FOURTH_QUARTER))){
					
		delta_angle = (uint16_t)fabs(dt-pulse_angle.angle);
				
		if(show_delta_angle){
						
			if(delta_angle_max < delta_angle)							
				delta_angle_max = delta_angle;
					
			display_send_num((int)delta_angle, 4, 9, 1);
			display_send_num((int)delta_angle_max, 4, 13, 1);		
						
		}
										
	}
	
}


 /**
  * @brief angle synchronization function
  * @param None
  * @retval None
  */
void SynchrAngle(void){
	
	if(pulse_angle.synchr){	
				
		pulse_angle.synchr = SYNCHR_OFF;
		pulse_angle.angle = (float)dt;
		delta_angle_max = 0;
				
	}
	
}


 /**
  * @brief Function for changing the rotation speed and smooth start of the stepper motor
  * @param None
  * @retval None
  */
void MotorSmoothStart(void){
	
	motor_step_period_change(rpm[current_cnt_encoder], &current_rpm_val);
	
}



 /**
  * @brief External pin interrupt handler: receiving a radio packet or pressing an encoder button
  * @param GPIO_Pin pin of the interruption that occurred
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin	== IRQ_PIN_NRF)
	{
		
		NrfRxCallbackHandler();
		if(is_rpm_editing){				
			ShowEditMenuEcnoder();				
		}
		
	}else if(GPIO_Pin	== IRQ_PIN_ENCODER){	
	
		PressButtonCallbackHandler();		
		
	}
	
	
}


 /**
  * @brief stand initialization main function
  * @param None
  * @retval None
  */
void init_stend(){
	
	InitLcd();
	InitRadioChanel();
	InitStepMotor();
	ENABLE_IT_EXTI_IRQ_ENCODER;
	
}

