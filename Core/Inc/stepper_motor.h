#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include "main.h"

/**структура таймера
  *pulse_count - счетик импульсов 
  *angle - угол поворота вала
  *1 импульс 0,1125 градуса
*/
typedef struct {
   
    float angle;
  	uint8_t synchr ;
	
}tim_count;

enum {
	SYNCHR_ON = 1,
	SYNCHR_OFF = 0
};

void motor_soft_start(void);
void motor_soft_stop(void);
 
 
	
extern tim_count pulse_angle ;

#endif

