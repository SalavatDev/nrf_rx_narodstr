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
 * @file    LCD_HD44780.h
 * @brief   module for encoder operation
 * @author  Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
 * 	The LCD screen library contains basic control, configuration and initialization functions.
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


#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_



/************************************
 * INCLUDES							
 ************************************/
#include "main.h"
#include <stdio.h>
#include "NRF24.h"


#ifdef __cplusplus
extern "C" {
#endif



/************************************
 * MACROS AND DEFINES
 ************************************/
 
#define LCD_ADDRESS                    0x4E
#define I2C_TIMEOUT                    100
#define ON                             1
#define OFF                            0
#define send_command                   0
#define send_data                      1
#define cursor_on                      1
#define cursor_off                     0
#define blink_on                       1
#define blink_off                      0





/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
 
void LCD_HD44780_init(void);    
void display_send_char(char *CHAR);
void goto_xy(uint8_t x, uint8_t y);
void display_send_num(int num, uint8_t dig_num, uint8_t pos_x, uint8_t pos_y);
void start_screen(void);
void main_menu(void);
void display_clear_text(void);

#ifdef __cplusplus
}
#endif

#endif 

