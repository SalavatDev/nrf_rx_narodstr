/* Библиотека дисплея HD44780 I2C
 *
 *
 *
*/


#include "LCD_HD44780.h"
#include <stdio.h>
#include "NRF24.h"

uint8_t i2c_data;
uint8_t backlight_status;
uint8_t rs_status;
extern I2C_HandleTypeDef hi2c1;


/*инициазизация дисплея*/
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

}

void LCD_Strobe_E(void){
    i2c_data |=0x04;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);    //поднимаем строб
    i2c_data &=0xF3 | backlight_status;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);    //снимаем строб
}

void display_on_off(uint8_t on_off){
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

void display_clear(void){
        i2c_data = 0x00;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
        DelayMicro(100);
        i2c_data = 0x10;
        HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &i2c_data, 1, I2C_TIMEOUT);
        LCD_Strobe_E();
        HAL_Delay(4);
}

void display_clear_text(void){
	
	char tmpstr[16]={"               "};
	tmpstr[15] = '\0';
	
	goto_xy(0, 0);   
	display_send_char(tmpstr); 
	
	goto_xy(0, 1);
	display_send_char(tmpstr);
	
	
}

void display_backligt(uint8_t on_off){
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
    
    char digit[num_of_digits];
    snprintf(digit, ++num_of_digits, "%d", num);
    goto_xy(pos_x, pos_y);
    display_send_char(digit);
}

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

void cursor_settings(uint8_t cursor, uint8_t blink){
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

void start_screen(void){
  goto_xy(0, 0);
  display_send_char("Rotaition tester");
  goto_xy(4, 1);
  display_send_char("TECHOFS");
}

void main_menu(void){
	
	goto_xy(0, 0);
	display_send_char("9B");
	
	goto_xy(3, 0);	
	display_send_char("12B");
	
	goto_xy(7, 0);	
	display_send_char("13B");
	
	goto_xy(12, 0);	
	display_send_char("D");
	
}

