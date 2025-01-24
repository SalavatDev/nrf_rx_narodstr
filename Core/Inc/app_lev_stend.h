#ifndef APP_STEND_H_
#define APP_STEND_H_

#include "main.h"

#include "tim.h"
#include "NRF24.h"
#include "LCD_HD44780.h"
#include "stepper_motor.h"
#include "encoder.h" 
#include 	<math.h>
 
extern union un_field_struct nrf_bits_field_rxdata;
extern uint16_t dt, delta_angle, delta_angle_max ;
 
void init_stend(void);

#endif

