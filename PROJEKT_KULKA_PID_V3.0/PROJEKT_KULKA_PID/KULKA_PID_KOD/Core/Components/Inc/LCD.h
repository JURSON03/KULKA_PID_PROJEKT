/*
 * LCD.h
 *
 *  Created on: Dec 11, 2025
 *      Author: juras
 */

#ifndef COMPONENTS_INC_LCD_H_
#define COMPONENTS_INC_LCD_H_

#include "main.h"
#include "SG90.h"
#include "HCSR04.h"
#include "PID.h"
#include "string.h"

typedef struct {

char uchyb_text[10];
char wartosc_zadana_text[10];

} LCD_Screen_HandleType;


extern LCD_Screen_HandleType LCD_Screen;



// void LCD_Screen_Init(LCD_Screen_HandleType* LCD_screen);



void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);






#endif /* COMPONENTS_INC_LCD_H_ */





















/*

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_


void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);

*/




