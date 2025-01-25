 

#include "stepper_motor.h"


#define freq TIM1->ARR
extern 	TIM_HandleTypeDef htim1;
 
StepMotorAngle pulse_angle = {0.0, 0};
uint8_t quarter_reciver = FIRST_SECOND_QUARTER;
 

uint8_t change_period_step = DISABLE;
static uint8_t is_tim_it_enabled = 0;
uint8_t show_delta_angle = DISABLE;

const float angle_step = 0.05625;


void DelayPeriodStepMks(__IO uint32_t us)
{
		CNT_TIM_DELAY_MOTOR_STEP = 0;	
    while(CNT_TIM_DELAY_MOTOR_STEP < us);
}


/**В Callback функции инкрементируем количество импульсов
  *6400 т.к. на один импульс происходит 2 вызова функции
  *при достижении значения 6400 (оборот 360 град.) обнуляем счетчик импульсов
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

 
/*процедура плавного старта двигателя, для исключения пропуска шагов*/
 
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


  


