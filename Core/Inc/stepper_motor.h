#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

 
#include "main.h"

#define	RIGHT_DIR	1

 
typedef struct {
   
    float angle;  
  	uint8_t synchr ;
	
}tim_count;

enum {
	SYNCHR_ON = 1,
	SYNCHR_OFF = 0
};


enum {
	
	RPM_1 = 0xFFFF,
	RPM_15 = 0x1194,	//4500
	RPM_30 = 0x08CA,	//2250
	RPM_60 = 0x0465,	//1125
	RPM_90 = 0x02ED,  //749
	RPM_120 = 0x0232 	//562
	
};

 
void motor_step_period_change(void);
void DelayPeriodStep(__IO uint32_t us);
extern uint8_t is_rpm_editing;
extern uint8_t current_cnt_encoder;
extern tim_count pulse_angle;
extern const uint16_t rpm_val[];
extern uint8_t is_change_period_step;

#endif

