
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
 * @file    encoder.c
 * @brief   module for encoder operation
 * @author 	Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
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


/************************************
 * INCLUDES
 ************************************/
#include "encoder.h"

static int16_t rd_cnt_encoder = 0;
const uint16_t rpm[SIZE_RPM_VAL] = {RPM_0, RPM_1, RPM_15, RPM_30, RPM_60, RPM_90, RPM_120};
const uint16_t rpm_val[SIZE_RPM_VAL] = {0, 1, 15, 30, 60, 90, 120};

uint16_t	current_rpm_val = RPM_1;
uint8_t		current_cnt_encoder = 0;
uint8_t 	is_rpm_editing = 0;

 
 
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	
	if(rd_cnt_encoder < (int16_t)CNT_TIM_ENCODER){
		
		if(current_cnt_encoder < (SIZE_RPM_VAL-1))
			current_cnt_encoder++;
			
	}else if(rd_cnt_encoder > (int16_t)CNT_TIM_ENCODER){
	 
		if(current_cnt_encoder > 0)
			current_cnt_encoder--;
		
	}
	
	 
  rd_cnt_encoder = (int16_t)CNT_TIM_ENCODER;
	
 
	display_send_num((int)rpm_val[current_cnt_encoder], 3, 0, 1);
	
  DelayMicro(65000); DelayMicro(65000);
	

}
 



void PressButtonCallbackHandler(){
	
		DISABLE_IT_EXTI_IRQ_NRF;
		DISABLE_IT_EXTI_IRQ_ENCODER;  
		
		show_delta_angle = DISABLE;
		is_rpm_editing = (!is_rpm_editing);
		
		if(is_rpm_editing){
			
			START_IT_TIM_ENCODEER;
			
			ShowEditMenuEcnoder();			
			
		}else{
			
			 
		  STOP_IT_TIM_ENCODEER;			
		   
			change_period_step = ENABLE;
			display_clear_text();
			main_menu(); 
 	
			ENABLE_IT_EXTI_IRQ_NRF;
			
		}
		
		 
		DELAY_TIM_ANTI_DREBEZG;
		
		 __HAL_GPIO_EXTI_CLEAR_IT(IRQ_PIN_ENCODER);
		
		ENABLE_IT_EXTI_IRQ_ENCODER;
		
}


 


