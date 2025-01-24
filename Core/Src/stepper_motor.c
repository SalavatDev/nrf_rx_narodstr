/** Реализация управления шаговым двигателем.
  * Аппаратно установлен режим микрошага 1/16, 0,1125 градуса-микрошаг
  * При таком микрошаге число импульсов на оборот = 3200
  * motor_soft_start - процедура плавного старта ШД, для исключения пропуска шагов
  * motor_soft_stop - процедура плавной остановки ШД
  */


#include "stepper_motor.h"
#include "NRF24.h"
#include "LCD_HD44780.h"
#include "tim.h"
#include <math.h>
#include "encoder.h"

#define freq TIM1->ARR
extern 	TIM_HandleTypeDef htim1;
 
StepMotorAngle pulse_angle = {0.0, 0};
uint8_t quarter_reciver = FIRST_SECOND_QUARTER;
 

uint8_t change_period_step = DISABLE;
uint8_t if_first_start_tim = YES;
uint8_t show_max_delta_angle = DISABLE;

const float angle_step = 0.05625;


void DelayPeriodStepMks(__IO uint32_t us)
{
		CNT_TIM_FOR_DELAY = 0;	
    while(CNT_TIM_FOR_DELAY < us);
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
void motor_step_period_change(void){
	
	if(change_period_step){		
		
		change_period_step = DISABLE;
		
		uint16_t new_rpm_val = rpm[current_cnt_encoder];
		
		pulse_angle.synchr = SYNCHR_ON;
	
		
		if(current_rpm_val > new_rpm_val){
			
			for(uint32_t i = current_rpm_val; i >= new_rpm_val; i--){
		 
				freq = i;
 
				DelayPeriodStepMks((uint32_t)((float)1/i * (float)10000000));	
				current_rpm_val--;
				if(change_period_step){
					show_max_delta_angle = DISABLE;
					break;					
				}
				
			}
			
		}else if(current_rpm_val < new_rpm_val){
			
			for(uint32_t i = current_rpm_val; i <= new_rpm_val; i++){
		 
				freq = i;
 
			 DelayPeriodStepMks((uint32_t)((float)1/i * (float)10000000));
				current_rpm_val++;
				if(change_period_step){
					show_max_delta_angle = DISABLE;
					break;					
				}
				
			}
			
		}
	 
		pulse_angle.synchr = SYNCHR_ON; 
		show_max_delta_angle = ENABLE;
		
	}

}


  


