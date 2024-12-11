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
	RPM_15 = 0x1194,
	RPM_30 = 0x08CA,
	RPM_60 = 0x0465,
	RPM_90 = 0x02ED,
	RPM_120 = 0x0232
	
};


 

void motor_soft_start(void);
void motor_soft_stop(void);
 
extern uint8_t is_rpm_editing;
extern uint8_t cnt_encoder;
extern tim_count pulse_angle;
extern uint16_t rpm_val[];

#endif

