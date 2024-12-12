/** ���������� ���������� ������� ����������.
  * ��������� ���������� ����� ��������� 1/16, 0,1125 �������-��������
  * ��� ����� ��������� ����� ��������� �� ������ = 3200
  * motor_soft_start - ��������� �������� ������ ��, ��� ���������� �������� �����
  * motor_soft_stop - ��������� ������� ��������� ��
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
/**� Callback ������� �������������� ���������� ���������
  *6400 �.�. �� ���� ������� ���������� 2 ������ �������
  *��� ���������� �������� 6400 (������ 360 ����.) �������� ������� ���������
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

 
/*��������� �������� ������ ���������, ��� ���������� �������� �����*/
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



/*��������� ������� ��������� ���������*/
void motor_soft_stop(void){ 

	for(int i = 1124; i <= 20000; i++){
	 
		freq = i;
		HAL_Delay(1);
		
	}
	
	HAL_TIM_Base_Stop_IT(&htim1);;
 
	
}

 

 



 


