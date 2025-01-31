/*******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Technology OFS.
 * All rights reserved.
 *
 * The information contained herein is confidential
 * property of Technology OFS.
 * The use, copying, transfer or disclosure of such
* information is prohibited except by express written
 * agreement with Technology OFS.
 * ******************************************************************************
 * @file    app_lev_stend.h
 * @brief   module for encoder operation
 * @author  Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
 *   This module is for defining and implementing top-level functions.
 *
 * Need to perform :
 *     TODO 
 * 
 * Log: 
 *     Version 1.0.0.0:  
 *         
 * 
 * Known Bugs:
 *       
 * 
 * Fixed Bugs:
 *     BUG FIXED dd.mm.yyyy 431 Description
 * 
 ******************************************************************************/

#ifndef APP_STEND_H_
#define APP_STEND_H_


/************************************
 * INCLUDES							
 ************************************/
#include "main.h"
#include "NRF24.h"
#include "LCD_HD44780.h"
#include "stepper_motor.h"
#include "encoder.h" 
#include <math.h>
 
 
#ifdef __cplusplus
extern "C" {
#endif
 
 
/************************************
 * EXPORTED VARIABLES
 ************************************/
 
extern uint16_t  delta_angle, delta_angle_max ;


/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void ShowAngleBitStat(void);
void ShowEditMenuEcnoder(void);
void SynchrAngle(void);
void MotorSmoothStart(void);
void init_stend(void);


#ifdef __cplusplus
}
#endif

#endif

