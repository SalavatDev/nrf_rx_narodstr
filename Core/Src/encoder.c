/** Реализация управления ?????????
  * Аппаратно установлен режим микрошага 1/16, 0,1125 градуса-микрошаг
  * При таком микрошаге число импульсов на оборот = 3200
  * motor_soft_start - процедура плавного старта ШД, для исключения пропуска шагов
  * motor_soft_stop - процедура плавной остановки ШД
  */


#include "encoder.h"


const uint16_t rpm[SIZE_RPM_VAL] = {RPM_1, RPM_15, RPM_30, RPM_60, RPM_90, RPM_120};
const uint16_t rpm_val[SIZE_RPM_VAL] = {1, 15, 30, 60, 90, 120};

uint16_t	current_rpm_val = RPM_1;
uint8_t		current_cnt_encoder = 0;
uint8_t 	is_rpm_editing = 0;

static int16_t rd_cnt_encoder = 0;
 
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
	
	//HAL_TIM_IRQHandler(&htim3);	
	
	 

}
 



 


