#ifndef __DISPLY_H__
#define __DISPLY_H__

#include <stdint.h>

void LCD_Init();
void function_set(unsigned char p_byte);
void commands(unsigned char p_byte);
void write(unsigned char p_byte);
void LCD_CHR(unsigned char p_byte);
unsigned char swap_byte(unsigned char p_byte);
void LCD_STR(char *p_string);
void LCD_ON();
void LCD_OFF();
void CUR_ON();
void CUR_OFF();
void CURB_ON();
void CURB_OFF();
void LCD_CLR();
void SET_DCB();
void LCD_RAM(unsigned char p_Adresse);
void LCD_INT(uint8_t p_Zahl);
void setPosition(uint8_t pos);

#endif
