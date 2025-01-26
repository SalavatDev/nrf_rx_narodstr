
#include "app_lev_stend.h"




static void InitLcd(void){
	
	START_TIM_DELAY_NRF;	
	LCD_HD44780_init();
	HAL_Delay(10);
	start_screen();
	display_clear_text();
	main_menu();
	
}


static void InitRadioChanel(void){
	
	Nrf24Init();
	
	ENABLE_IT_EXTI_IRQ_NRF;
	
}


void InitStepMotor(void){
	
	if(RIGHT_DIR)
		HAL_GPIO_WritePin(DIR_ROTATION_GPIO_Port, DIR_ROTATION_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DIR_ROTATION_GPIO_Port, DIR_ROTATION_Pin, GPIO_PIN_RESET);
	
	START_TIM_DELAY_CHANGE_STEP;
 	
}



void ShowEditMenuEcnoder(void){
	
	display_clear_text();
	goto_xy(0, 0);
	display_send_char("RPM");			 			 
	display_send_num((int)rpm_val[current_cnt_encoder], 3, 0, 1);
	
}


uint16_t delta_angle = 0, delta_angle_max = 0;

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

void SynchrAngle(void){
	
	if(pulse_angle.synchr){	
				
		pulse_angle.synchr = SYNCHR_OFF;
		pulse_angle.angle = (float)dt;
		delta_angle_max = 0;
				
	}
	
}

void MotorSmoothStart(void){
	
	motor_step_period_change(rpm[current_cnt_encoder], &current_rpm_val);
	
}

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
 
void init_stend(){
	
	InitLcd();
	InitRadioChanel();
	InitStepMotor();
	ENABLE_IT_EXTI_IRQ_ENCODER;
	
}

