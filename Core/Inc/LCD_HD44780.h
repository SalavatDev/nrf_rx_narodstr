#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

#include "main.h"

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



void LCD_HD44780_init(void);   //инициазизация дисплея
void LCD_Strobe_E(void);       //отправка стробирующего импульса на вход Е
void display_on_off(uint8_t on_off);
void display_clear(void);
void display_backligt(uint8_t on_off);
void display_send_char(char *CHAR);
void goto_xy(uint8_t x, uint8_t y);
void cursor_settings(uint8_t cursor, uint8_t blink);
void display_send_num(int num, uint8_t dig_num, uint8_t pos_x, uint8_t pos_y);
void start_screen(void);
void main_menu(void);
void display_clear_text(void);


#endif 

