/** Реализация управления шаговым двигателем.
  * Аппаратно установлен режим микрошага 1/16, 0,1125 градуса-микрошаг
  * При таком микрошаге число импульсов на оборот = 3200
  * motor_soft_start - процедура плавного старта ШД, для исключения пропуска шагов
  * motor_soft_stop - процедура плавной остановки ШД
  */


#include "stepper_motor.h"
#include "NRF24.h"

#define freq TIM1->ARR
extern 	TIM_HandleTypeDef htim1;
 
tim_count pulse_angle = {0.0, 0};

uint16_t rpm[6] = {RPM_1, RPM_15, RPM_30, RPM_60, RPM_90, RPM_120};
uint16_t rpm_val[6] = {1, 15, 30, 60, 90, 120};

uint16_t current_rpm_val = RPM_1;
uint8_t current_cnt_encoder = 0;
uint8_t is_rpm_editing = 0;

uint8_t is_change_period_step = 0;
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
				pulse_angle.angle += 0.05625;
			
			}else{
			
				if(pulse_angle.angle <= 0.0) 
				{
					pulse_angle.angle = 360.0;			 
				}		
				pulse_angle.angle -= 0.05625;
			
		}
		
		HAL_GPIO_TogglePin(STEPPER_GPIO_Port, STEPPER_Pin);
 
}

 
/*процедура плавного старта двигателя, для исключения пропуска шагов*/
void motor_step_period_change(void){
	 
	pulse_angle.synchr = SYNCHR_ON;
	HAL_TIM_Base_Start_IT(&htim1);
	
	if(current_rpm_val > rpm_val[current_cnt_encoder]){
		
		for(int i = current_rpm_val; i >= rpm_val[current_cnt_encoder]; i--){
	 
			freq = i;
			HAL_Delay(1);
		
		}
		
	}else if(current_rpm_val < rpm_val[current_cnt_encoder]){
		
		for(int i = current_rpm_val; i <= rpm_val[current_cnt_encoder]; i--){
	 
			freq = i;
			HAL_Delay(1);
		
		}
		
	}
 
	pulse_angle.synchr = SYNCHR_ON; 
 
}



/*процедура плавной остановки двигателя*/
void motor_soft_stop(void){ 

	for(int i = 1124; i <= 20000; i++){
	 
		freq = i;
		HAL_Delay(1);
		
	}
	
	HAL_TIM_Base_Stop_IT(&htim1);;
 
	
}

 

 



 


