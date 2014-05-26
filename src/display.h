#ifndef __DISPLY_H__
#define __DISPLY_H__

#include <stdint.h>

#define TRUE 		1
#define FALSE 0
#define RS 			4
#define E 			5
#if !defined(LCD) || !defined(LCD_D)
#define LCD 		PORTA
#define LCD_D		DDRA
#endif
#define DISPLAY 	2
#define CURSOR 		1
#define CURSOR_B 	0
#define NUMBEROFCHRFIELDS 64

void LCD_Init();
void function_set(uint8_t p_byte);
void commands(uint8_t p_byte);
void write(uint8_t p_byte);
void LCD_CHR(uint8_t p_byte);
uint8_t swap_byte(uint8_t p_byte);
void LCD_STR(char *p_string);
void LCD_ON();
void LCD_OFF();
void CUR_ON();
void CUR_OFF();
void CURB_ON();
void CURB_OFF();
void LCD_CLR();
void SET_DCB();
void LCD_RAM(uint8_t p_Adresse);
void LCD_INT(uint8_t p_Zahl);
void setPosition(uint8_t pos);
void setPosition_XY(uint8_t posX, uint8_t posY);
void LCD_CLR_POS(uint8_t p_Position);
void LCD_CLR_SIGN();
void LCD_POS_DOWN();
void LCD_POS_UP();
void LCD_BLACK_SIGN();
void LCD_BLACK_POS(uint8_t p_Position);
void LCD_BLACK_FULL();
void LCD_POS_RIGHT();
void LCD_POS_LEFT();
void LCD_changeNumber(uint8_t pUpDown);
void LCD_changeNumberPos(uint8_t pos, uint8_t pUpDown);
void LCD_OutTwoNumbers(uint8_t intNumber);
uint8_t getPosition();


#endif