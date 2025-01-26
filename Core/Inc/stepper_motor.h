#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

 
#include "main.h"
#include "NRF24.h"
#include "LCD_HD44780.h"
//#include "tim.h"
#include <math.h>
#include "encoder.h"



#define	RIGHT_DIR	1


#define DELAY_TIM_ANTI_DREBEZG  {DelayMicro(65000); DelayMicro(65000);  DelayMicro(65000); }  //65ms*3=195 ms

#define	START_TIM_DELAY_CHANGE_STEP		HAL_TIM_Base_Start(&htim4)
#define	CNT_TIM_DELAY_MOTOR_STEP 	TIM4->CNT

#define	START_IT_TIM_STEPPER		HAL_TIM_Base_Start_IT(&htim1)
#define	STOP_IT_TIM_STEPPER			HAL_TIM_Base_Stop_IT(&htim1)

 


enum{
	
	YES = 0,
	NO = 1
	
};
 
typedef struct {
   
    float angle;  
  	uint8_t synchr ;
	
}StepMotorAngle;

enum {
	SYNCHR_ON = 1,
	SYNCHR_OFF = 0
};

enum {
	
	FIRST_SECOND_QUARTER = 0,
	THIRD_FOURTH_QUARTER = 1,
	ANOTHER_QUARTER = 2
};



void motor_step_period_change(uint16_t new_rpm_val, uint16_t *current_rpm_val);
void DelayPeriodStepMks(__IO uint32_t us);
extern uint8_t is_rpm_editing;
extern uint8_t current_cnt_encoder;
extern StepMotorAngle pulse_angle;
extern uint8_t change_period_step;
//extern uint8_t if_first_start_tim ;
extern uint8_t show_delta_angle;
extern uint8_t quarter_reciver;


#endif

