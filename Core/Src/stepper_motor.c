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
 
tim_count pulse_angle = {  0.0, 0};

//uint32_t cnt_pulse = 0;


/**В Callback функции инкрементируем количество импульсов
  *6400 т.к. на один импульс происходит 2 вызова функции
  *при достижении значения 6400 (оборот 360 град.) обнуляем счетчик импульсов
*/ 
 
 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	  
		if(pulse_angle.angle >= 360.0) 
		{
			pulse_angle.angle = 0.0;			 
		}
		
		pulse_angle.angle += 0.05625;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		/*
		cnt_pulse++;
		if(cnt_pulse == 6400){
			cnt_pulse = 0;
			
		}
	*/
}

/*
static void tgl_soft_pwm(uint32_t *del){
	
	while(1)
	{
		
			GPIOB->BSRR = GPIO_PIN_0;
			DelayMicro(*del);		
			GPIOB->BRR = GPIO_PIN_0;
			DelayMicro(*del);
			pulse_angle.angle += 0.1125; 
			
			if(pulse_angle.angle >= 360.0) 
			{
				pulse_angle.angle = 0.0;			 
			}
			
	}
	
}		
 */
/*
static void motor_turn(uint32_t sec){
	
	uint32_t rpm = 3200*sec;
	
	for(uint32_t i=0;i<rpm;++i){
	 
		GPIOB->BSRR = GPIO_PIN_0;
		DelayMicro(156);;		
		GPIOB->BRR = GPIO_PIN_0;
		DelayMicro(156);;
		pulse_angle.angle += 0.1125; 
		
		if(pulse_angle.angle >= 360.0) 
		{
			pulse_angle.angle = 0.0;			 
		}
 
	}
} 
*/

/*процедура плавного старта двигателя, для исключения пропуска шагов*/
void motor_soft_start(void){
	 
	pulse_angle.synchr = SYNCHR_ON;
	HAL_TIM_Base_Start_IT(&htim1);
	
	for(int i = 20000; i >= 1124; i--){
	 
		freq = i;
		HAL_Delay(1);
	}
	 
	pulse_angle.synchr = SYNCHR_ON;
  
	
 /*

	pulse_angle.synchr = SYNCHR_ON;
 
  
	 for(uint16_t i=5000; i >= 156; i--){
    
		GPIOB->BSRR = GPIO_PIN_0;
		DelayMicro(i);		
		GPIOB->BRR = GPIO_PIN_0;
		DelayMicro(i);
		pulse_angle.angle += 0.1125; 
		
		if(pulse_angle.angle >= 360.0) 
		{
			pulse_angle.angle = 0.0;			 
		}
		
		//DelayMicro(100);
 
	}
	
	 
	for(uint32_t i=0;i<32000;++i){
	 
		GPIOB->BSRR = GPIO_PIN_0;
		DelayMicro(156);;		
		GPIOB->BRR = GPIO_PIN_0;
		DelayMicro(156);;
		pulse_angle.angle += 0.1125; 
		
		if(pulse_angle.angle >= 360.0) 
		{
			pulse_angle.angle = 0.0;			 
		}
 
	}
	*/
}



/*процедура плавной остановки двигателя*/
void motor_soft_stop(void){ 

	for(int i = 1124; i <= 20000; i++){
	 
		freq = i;
		HAL_Delay(1);
		
	}
	
	HAL_TIM_Base_Stop_IT(&htim1);;

 /*
	for(uint16_t i=157; i <= 1000;++i ){
		
		GPIOB->BSRR = GPIO_PIN_0;
		DelayMicro(i);		
		GPIOB->BRR = GPIO_PIN_0;
		DelayMicro(i);
		pulse_angle.angle += 0.1125; 
		
		if(pulse_angle.angle >= 360.0) 
		{
			pulse_angle.angle = 0.0;			 
		}
	}
	
	for(uint16_t i=1; i <= 20; ++i ){
		
		GPIOB->BSRR = GPIO_PIN_0;
		HAL_Delay(i);		
		GPIOB->BRR = GPIO_PIN_0;
		HAL_Delay(i);
		pulse_angle.angle += 0.1125; 
		
		if(pulse_angle.angle >= 360.0) 
		{
			pulse_angle.angle = 0.0;			 
		}
		
	}
 */
	
}

 

 



 


