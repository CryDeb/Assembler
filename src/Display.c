#include <avr/io.h>
#include <util/delay.h>
#include  "display.h"

#define TRUE 		1
#define RS 			4
#define E 			5
#define LCD 		PORTC
#define LCD_D		DDRC
#define DISPLAY 	2
#define CURSOR 		1
#define CURSOR_B 	0

unsigned char S_LCD = 7;
uint8_t position = 0;
/**************************************************************************
*
* Funktion: 	LCD_Init
* Paramteter:	-
*
* Beschreibung: Initialisert das Display ein
* 
*
**************************************************************************/

void LCD_Init(){
	LCD_D = 0xFF;
	position = 0;
	unsigned char byte;
	

	_delay_ms(20);
	byte = 0b00000011;
	function_set(byte);	//Set DL at High
	_delay_ms(10);
	byte = 0b00000011;
	function_set(byte);	//Set DL at High
	_delay_ms(10);
	byte = 0b00000011;
	function_set(byte);	//Set DL at High
	_delay_ms(10);
	byte = 0b00000010;
	function_set(byte);	//Set DL at High
	_delay_ms(10);
	byte = 0b00101000;	
	commands(byte);		//Set DL at High. Set Display Line Number and character Font
	_delay_ms(10);
	byte = 0b00001111;
	commands(byte);	
	_delay_ms(10);
	byte = 0b00000001;
	commands(byte);			//Clear Display
	_delay_ms(10);
	byte = 0b00000110;
	commands(byte);			//Set Shift Modes
	_delay_ms(10);
}

/**************************************************************************
*
* Funktion: 	function_set
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void function_set(unsigned char p_byte){
	LCD &= ~(1 << RS);
	LCD = p_byte;
	LCD |= (1 << E);
	LCD &= ~(1 << E);
	LCD |= (1 << RS);
}

/**************************************************************************
*
* Funktion: 	commands
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void commands(unsigned char p_byte){
	unsigned char p_write;
	LCD &= ~(1 << RS);
	for(unsigned char Count = 0; Count < 2; Count ++)
	{
		p_byte = swap_byte(p_byte);
		p_write = p_byte & 0x0F;
		LCD = p_write;
		LCD |= (1 << E);
		LCD &= ~(1 << E);
	}
	_delay_ms(20);
	LCD |= (1 << RS);
}

/**************************************************************************
*
* Funktion: 	write
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void write(unsigned char p_byte){
	unsigned char p_write;
	LCD |= (1 << RS);
	for(unsigned char Count = 0; Count < 2; Count ++)
	{
		p_byte = swap_byte(p_byte);
		p_write = p_byte & 0x0F;
		p_write |= 0x10;
		LCD = p_write;
		LCD |= (1 << E);
		LCD &= ~(1 << E);
	}
	_delay_ms(20);
	LCD &= ~(1 << RS);
	//setPosition(position+1);
}

void setPosition(uint8_t pos){
	switch(pos){
		case 20:
		case 40:
			pos += 20;
			break;
		case 60:
			pos -= 40;
			break;
		case 80:
			pos = 0;
			break;
	}
	LCD_RAM(pos);
}

/**************************************************************************
*
* Funktion: 	commands
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void LCD_CHR(unsigned char p_byte){
	write(p_byte);
}

/**************************************************************************
*
* Funktion: 	swap_byte
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Tascuht die Nibble eines Bytes
*
**************************************************************************/
unsigned char swap_byte(unsigned char p_byte){
	unsigned char nibble1 = p_byte & 0x0F;
	unsigned char nibble2 = p_byte & 0xF0;
	nibble1 = nibble1 << 4;
	nibble2 = nibble2 >> 4;
	p_byte = nibble1 | nibble2;
	return p_byte;
}

/**************************************************************************
*
* Funktion: 	commands
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void LCD_STR(char *p_string)
{
	while(*p_string != 0) {
		LCD_CHR(*p_string);
		p_string++;
	}
}

/**************************************************************************
*
* Funktion: 	LCD_ON
*
* Beschreibung: Schaltet Display ein
*
**************************************************************************/
void LCD_ON()
{
	S_LCD |= (1 << DISPLAY);
	SET_DCB();
}

/**************************************************************************
*
* Funktion: 	LCD_OFF
*
* Beschreibung: Schaltet Display aus
*
**************************************************************************/
void LCD_OFF(){
	S_LCD &= ~(1 << DISPLAY);
	SET_DCB();
}

/**************************************************************************
*
* Funktion: 	CUR_ON
*
* Beschreibung: Schaltet Cursor ein
*
**************************************************************************/
void CUR_ON()
{
	S_LCD |= (1 << CURSOR);
	SET_DCB();
}

/**************************************************************************
*
* Funktion: 	CUR_OFF
*
* Beschreibung: Schaltet Cursor aus
*
**************************************************************************/
void CUR_OFF()
{
	S_LCD &= ~(1 << CURSOR);
	SET_DCB();
}

/**************************************************************************
*
* Funktion: 	CURB_ON
*
* Beschreibung: Schaltet Cursor ein
*
**************************************************************************/
void CURB_ON()
{
	S_LCD |= (1 << CURSOR_B);
	SET_DCB();
}

/**************************************************************************
*
* Funktion: 	CURB_OFF
*
* Beschreibung: Schaltet Cursor aus
*
**************************************************************************/
void CURB_OFF()
{
	S_LCD &= ~(1 << CURSOR_B);
	SET_DCB();
}

/**************************************************************************
*
* Funktion: 	LCD_CLR
*
* Beschreibung: Löscht das Display
*
**************************************************************************/
void LCD_CLR()
{		
	position = 0;
	commands(1);
}

/**************************************************************************
*
* Funktion: 	SET_DCB
*
* Beschreibung: Löscht das Display
*
**************************************************************************/
void SET_DCB()
{
	unsigned char write;
	write = S_LCD;
	write += 8;
	commands(write);
}

/**************************************************************************
*
* Funktion: 	LCD_RAM
*
* Beschreibung: Setzt Display-Ram-Adresse
*
**************************************************************************/
void LCD_RAM(uint8_t p_Adresse)
{
	/*if(p_Adresse >= 41){
		p_Adresse += 24;
	}
	position = p_Adresse;*/
	p_Adresse |= 0b10000000;
	commands(p_Adresse);
	_delay_ms(10);
}

/**************************************************************************
*
* Funktion: 	LCD_INT
*
* Beschreibung: Setzt Display-Ram-Adresse
*
**************************************************************************/
void LCD_INT(uint8_t p_Zahl)
{
	if(p_Zahl < 0)
	{
		p_Zahl *= -1;
		LCD_CHR('-');
	}
	unsigned char naZahl[50];
	unsigned char length = 0;
	naZahl[0] = 0b00110000;
	for(unsigned char Count = 0; p_Zahl > 0; Count ++)
	{
		unsigned char nZahl = p_Zahl % 10;
		nZahl |= 0b00110000;
		naZahl[Count] = nZahl;
		p_Zahl /= 10;
		length = Count;
	}
	for(unsigned char Count = length; Count > 0; Count --)
	{
		write(naZahl[Count]);
	}
	write(naZahl[0]);
}
