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
 * @file    LCD_HD44780.c
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

#include "LCD_HD44780.h"


 
/************************************
 * EXTERN VARIABLES
 ************************************/
extern I2C_HandleTypeDef hi2c1;
 
 
 
 
/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t i2c_data;
static uint8_t backlight_status;




/**
  * @brief auxiliary function for data and command transactions during 
	* 			 reading and writing
  * @param None
  * @retval None
  */
static void LCD_Strobe_E(void){
    i2c_data |=0x04;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);    //поднимаем строб
    i2c_data &=0xF3 | backlight_status;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);    //снимаем строб
}



/**
  * @brief on off lcd
  * @param on_off comand,  1 -  on, 0 - off
  * @retval None
  */
static void display_on_off(uint8_t on_off){
	
        i2c_data = 0x00;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
    if(on_off == 0){
        i2c_data = 0x80;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
    }
    else{
        i2c_data = 0xF0;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
    
    }
		
}



/**
  * @brief clear lcd
  * @param None
  * @retval None
  */
static void display_clear(void){
	
        i2c_data = 0x00;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
        DelayMicro(100);
        i2c_data = 0x10;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
        HAL_Delay(4);
	
}



/**
  * @brief turning on the reverse backlight of the screen
  * @param on_off comand,  1 -  on, 0 - off
  * @retval None
  */
static void display_backligt(uint8_t on_off){
	
    if(on_off){
        backlight_status = 0x08;
        i2c_data = backlight_status;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    }
    else{
        backlight_status = 0x00;
        i2c_data = backlight_status;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    }

}


/**
  * @brief setting up the cursor
  * @param cursor comand,  1 -  on, 0 - off
	* @param blink comand,  1 -  on, 0 - off
  * @retval None
  */
static void cursor_settings(uint8_t cursor, uint8_t blink){
    i2c_data = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();
    if(cursor){
        if(blink) i2c_data = 0xF0 | backlight_status;
        else i2c_data = 0xE0 | backlight_status;
    }
    else{
        if(blink) i2c_data = 0xD0 | backlight_status;
        else i2c_data = 0xC0 | backlight_status;
    }
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();

}



/**
  * @brief  clear text on lcd
  * @param None 
  * @retval None
  */
void display_clear_text(void){
	
	char tmpstr[17]={"                "};
	tmpstr[16] = '\0';
	
	goto_xy(0, 0);   
	display_send_char(tmpstr); 
	
	goto_xy(0, 1);
	display_send_char(tmpstr);
	
	
}


/**
  * @brief displaying a number on the screen
  * @param num number 
  * @param dig_num number of digits 
  * @param pos_x, pos_y coordinates of the number 
  * @retval None
  */
void display_send_num(int num, uint8_t dig_num, uint8_t pos_x, uint8_t pos_y){
    int i = num;
    uint8_t num_of_digits = 1;
    goto_xy(pos_x, pos_y);
        while(dig_num){
        display_send_char(" ");
        dig_num--;
    }
    
    while(i != 0){
        i /=10;
        num_of_digits++;
    }
    if(num == 0){
        goto_xy(pos_x, pos_y);
        display_send_char("0");
    }
    
		++num_of_digits;
    char digit[num_of_digits];
    snprintf(digit, num_of_digits, "%d", num);
    goto_xy(pos_x, pos_y);
    display_send_char(digit);
}


/**
  * @brief setting the screen coordinate
  * @param x, y coordinates 
  * @retval None
  */
void goto_xy(uint8_t x, uint8_t y){
    uint8_t data[4] = {0};
    uint8_t adress;
    if(y) adress = x + 0x40; 
    else  adress = x; 
    data[0] = (adress & 0xF0) | 0x84 | backlight_status;
    data[1] = (adress & 0xF0) | 0x80 | backlight_status;
    data[2] = ((adress & 0x0F)<<4) | 0x04 | backlight_status; 
    data[3] = ((adress & 0x0F)<<4) | backlight_status;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data, 4, I2C_TIMEOUT);
}




/**
  * @brief demonstration of the screen when turned on
  * @param None
  * @retval None
  */
void start_screen(void){
 
  goto_xy(0, 0);
  display_send_char("OFS Technologes");
  goto_xy(0, 1);
  display_send_char("Rotaition tester");
	HAL_Delay(3000);
	
}


/**
  * @brief displaying a character on the screen
  * @param str pointer at string
  * @retval None
  */
void display_send_char(char *str){
    uint8_t data[4] = {0};
    while(*str){
        data[0] = (*str & 0xF0) | 0x0D;
        data[1] = (*str & 0xF0) | 0x09;
        data[2] = ((*str & 0x0F)<<4) | 0x0D;
        data[3] = ((*str & 0x0F)<<4) | 0x09;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data, 4, I2C_TIMEOUT);
        str++;
    }
}


/**
  * @brief initializing the display
  * @param None
  * @retval None
  */
void LCD_HD44780_init(void){
    HAL_Delay(15);
    i2c_data = 0x30;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();
    HAL_Delay(5);
    LCD_Strobe_E();
    DelayMicro(100);
    LCD_Strobe_E();
    
    i2c_data = 0x20;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();
    
    LCD_Strobe_E();
    
    i2c_data = 0xC0;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();
    
    display_on_off(OFF);
    display_clear();
    
    i2c_data = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();
    
    i2c_data = 0x60;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
    LCD_Strobe_E();
    
    display_on_off(ON);
		HAL_Delay(50);
		cursor_settings(cursor_off, blink_off);
		
		HAL_Delay(1);
		display_backligt(ENABLE);

}
 

/**
  * @brief menu for displaying the status bits and the difference in the 
	*				angles of the magnetometer.
  * @param None
  * @retval None
  */
void main_menu(void){
	
	goto_xy(0, 0);
	display_send_char("9");
	
	goto_xy(2, 0);
	display_send_char("12");
 
	
	goto_xy(5, 0);	
	display_send_char("13");
	
	goto_xy(9, 0);	
	display_send_char("D");
	
	goto_xy(12, 0);	
	display_send_char("Dmax");
	
}

