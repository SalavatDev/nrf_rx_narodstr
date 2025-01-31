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
 * @file    stepper_motor.c
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

/************************************
 * INCLUDES
 ************************************/
#include "stepper_motor.h"
#include <cstdint>



/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define freq TIM1->ARR




/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t is_tim_it_enabled = 0;
static const float angle_step = 0.05625;




/************************************
 * GLOBAL VARIABLES
 ************************************/
StepMotorAngle pulse_angle = {0.0, 0};
uint8_t quarter_reciver = FIRST_SECOND_QUARTER;
uint16_t current_rpm_val = RPM_1;
uint8_t change_period_step = DISABLE;
uint8_t show_delta_angle = DISABLE;





/************************************
 * STATIC FUNCTIONS
 ************************************/

/**
  * @brief microsecond delay function
  * @param us number of microseconds
  * @retval None
  */
static void DelayPeriodStepMks(uint32_t us)
{
		CNT_TIM_DELAY_MOTOR_STEP = 0;	
    while(CNT_TIM_DELAY_MOTOR_STEP < us);
}





/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
  * @brief pwm timer interrupt handler
  * @param htim timer Handle TypeDef
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
		if(RIGHT_DIR){
			
				if(pulse_angle.angle >= 360.0) 
				{
					pulse_angle.angle = 0.0;			 
				}		
					
				pulse_angle.angle += angle_step;
			
			}else{
			
				if(pulse_angle.angle <= 0.0) 
				{
					pulse_angle.angle = 360.0;			 
				}	
				
				pulse_angle.angle -= angle_step;
			
		}
		
		if((pulse_angle.angle > 0.0)&&(pulse_angle.angle < 180.0)){
			
			quarter_reciver = FIRST_SECOND_QUARTER;
			
		}else{
			
			quarter_reciver = THIRD_FOURTH_QUARTER;
			
		}
		
		HAL_GPIO_TogglePin(STEPPER_GPIO_Port, STEPPER_Pin);

}


/**
  * @brief stepper motor smooth acceleration function
  * @param new_rpm_val new stepper motor speed value
	* @param current_rpm_val pointer to current stepper motor speed value
  * @retval None
  */
void motor_step_period_change(uint16_t new_rpm_val, uint16_t *current_rpm_val){
	
	if(change_period_step){		

		show_delta_angle = DISABLE;
		change_period_step = DISABLE;
		uint16_t new_rpm_val_ = new_rpm_val;	  
		
		pulse_angle.synchr = SYNCHR_ON;
	

		if(!new_rpm_val_){
			
			for(uint32_t i = *current_rpm_val; i <= 0xC350; i++){
		
				freq = i; 
				DelayPeriodStepMks((uint32_t)((float)1/i * (float)10000000));		
				if(change_period_step){					
					break;					
				}
				
			}
			
			*current_rpm_val = RPM_1;
			STOP_IT_TIM_STEPPER;  
			is_tim_it_enabled = DISABLE;

			
		}else {
			
			if(!is_tim_it_enabled){
			
				START_IT_TIM_STEPPER;
				is_tim_it_enabled = ENABLE;
			
			}
		
			if(*current_rpm_val > new_rpm_val_){
				
				for(uint32_t i = *current_rpm_val; i >= new_rpm_val_; i--){
			
					freq = i;
	
					DelayPeriodStepMks((uint32_t)((float)1/i * (float)10000000));	
					(*current_rpm_val)--;
					if(change_period_step){					
						break;					
					}
					
				}
				
								
			}else if(*current_rpm_val < new_rpm_val_){
				
				for(uint32_t i = *current_rpm_val; i <= new_rpm_val_; i++){
			
					freq = i;
	
				 DelayPeriodStepMks((uint32_t)((float)1/i * (float)10000000));
					(*current_rpm_val)++;
					if(change_period_step){						
						break;					
					}
					
				}
				
			}
			
			pulse_angle.synchr = SYNCHR_ON; 
			show_delta_angle = ENABLE;
		
		}
		
		
	}

}



