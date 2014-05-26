#include <avr/io.h>
//#include <util/delay.h>
#include "display.h"

uint8_t S_LCD = 7;
uint8_t LCD_current_position = 0;
uint8_t LCD_Info[NUMBEROFCHRFIELDS];
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
	LCD_current_position = 0;
	uint8_t byte;
	//_delay_ms(20);
	W10ms();
	W10ms();
	byte = 0b00000011;
	function_set(byte);	//Set DL at High
	//_delay_ms(10);
	W10ms();
	byte = 0b00000011;
	function_set(byte);	//Set DL at High
	//_delay_ms(10);
	W10ms();
	byte = 0b00000011;
	function_set(byte);	//Set DL at High
	//_delay_ms(10);
	W10ms();
	byte = 0b00000010;
	function_set(byte);	//Set DL at High
	//_delay_ms(10);
	W10ms();
	byte = 0b00101000;	
	commands(byte);		//Set DL at High. Set Display Line Number and character Font
	//_delay_ms(10);
	W10ms();
	byte = 0b00001111;
	commands(byte);	
	//_delay_ms(10);
	W10ms();
	byte = 0b00000001;
	commands(byte);			//Clear Display
	//_delay_ms(10);
	W10ms();
	byte = 0b00000110;
	commands(byte);			//Set Shift Modes
	//_delay_ms(10);
	W10ms();
	CURB_OFF();
}

/**************************************************************************
*
* Funktion: 	function_set
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void function_set(uint8_t p_byte){
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
void commands(uint8_t p_byte){
	uint8_t p_write;
	LCD &= ~(1 << RS);
	for(uint8_t Count = 0; Count < 2; Count ++)
	{
		p_byte = swap_byte(p_byte);
		p_write = p_byte & 0x0F;
		LCD = p_write;
		LCD |= (1 << E);
		LCD &= ~(1 << E);
	}
	//_delay_ms(20);
	W10ms();
	W10ms();
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
void write(uint8_t p_byte){
	setPosition(LCD_current_position);
	uint8_t p_write;
	LCD |= (1 << RS);
	for(uint8_t Count = 0; Count < 2; Count ++)
	{
		p_byte = swap_byte(p_byte);
		p_write = p_byte & 0x0F;
		p_write |= 0x10;
		LCD = p_write;
		LCD |= (1 << E);
		LCD &= ~(1 << E);
	}
	LCD_Info[LCD_current_position]=p_byte;
	//_delay_ms(20);
	W10ms();
	W10ms();
	LCD &= ~(1 << RS);
	LCD_current_position++;
}

void setPosition(uint8_t pos){
	LCD_current_position=pos;
	if(pos>=0&&pos<=15){		// 0-15
		LCD_RAM((pos));
	}else if(pos>=32&&pos<=47){	// 16-32
		LCD_RAM((pos-16));
	}else if(pos>=16&&pos<=31){	// 40-79
		LCD_RAM((pos+48));
	}else if(pos>=48&&pos<=63){	// 80-96
		LCD_RAM((pos+32));
	}else{
		LCD_CLR();
		LCD_RAM((0));
		LCD_current_position=0;
	}
}
void setPosition_XY(uint8_t posX, uint8_t posY){
    setPosition((posY*16+posX));
}
uint8_t getPositionCHR(uint8_t pos){
    return LCD_Info[pos];
}
uint8_t getPosition(){
    return LCD_current_position;
}
/**************************************************************************
*
* Funktion: 	commands
* Paramteter:	unsigned char p_byte
*
* Beschreibung: Berechnet den Wert aus einer Hochrechnung aus
*
**************************************************************************/
void LCD_CHR(uint8_t p_byte){
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
uint8_t swap_byte(uint8_t p_byte){
	uint8_t nibble1 = p_byte & 0x0F;
	uint8_t nibble2 = p_byte & 0xF0;
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
	LCD_current_position = 0;
	for(uint8_t count=0; count < NUMBEROFCHRFIELDS; count++){
	    LCD_Info[count]=0;
	}
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
	uint8_t write;
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
	LCD_current_position = p_Adresse;*/
	p_Adresse |= 0b10000000;
	commands(p_Adresse);
	//_delay_ms(10);
	W10ms();
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
	uint8_t naZahl[50];
	uint8_t length = 0;
	naZahl[0] = 0b00110000;
	for(uint8_t Count = 0; p_Zahl > 0; Count ++)
	{
		uint8_t nZahl = p_Zahl % 10;
		nZahl |= 0b00110000;
		naZahl[Count] = nZahl;
		p_Zahl /= 10;
		length = Count;
	}
	for(uint8_t Count = length; Count > 0; Count --)
	{
		write(naZahl[Count]);
	}
	write(naZahl[0]);
}
void LCD_CLR_POS(uint8_t p_Position){
	uint8_t tmpPos = LCD_current_position;
	setPosition(p_Position);
	LCD_CHR(' ');
	setPosition(tmpPos);
}
void LCD_CLR_SIGN(){
	LCD_CHR(' ');
	setPosition(LCD_current_position-1);
}
void LCD_POS_UP(){
	if(LCD_current_position>15){
	    setPosition(LCD_current_position-16);
	}
}
void LCD_POS_DOWN(){
	if(LCD_current_position<48){
	    setPosition(LCD_current_position+16);
	}
}
void LCD_POS_LEFT(){
	if(LCD_current_position%16 != 0){
	    setPosition(LCD_current_position-1);
	}
}
void LCD_POS_RIGHT(){
	if((LCD_current_position+1)%16 != 0){
	    setPosition(LCD_current_position+1);
	}
	
}
void LCD_BLACK_SIGN(){
	write(255);
}
void LCD_BLACK_POS(uint8_t p_Position){
	uint8_t tmpPos = LCD_current_position;
	setPosition(p_Position);
	write(255);
	setPosition(tmpPos);
}
void LCD_changeNumber(uint8_t pUpDown){
    if(LCD_Info[LCD_current_position] >= '0' && LCD_Info[LCD_current_position] <='9'){
	switch (pUpDown){
	    case 0:
		if(LCD_Info[LCD_current_position] == '9'){
		    LCD_CLR_SIGN();
		    LCD_CHR('0');
		}else{
		    LCD_CHR(LCD_Info[LCD_current_position]+1);
		}
		setPosition(LCD_current_position-1);
		break;
	    case 1:
		if(LCD_Info[LCD_current_position] == '0'){
		    LCD_CLR_SIGN();
		    LCD_CHR('9');
		}else{
		    LCD_CHR(LCD_Info[LCD_current_position]-1);
		}
		setPosition(LCD_current_position-1);		
		break;
	}
    }
}
void LCD_changeNumberPos(uint8_t pos, uint8_t pUpDown){
    uint8_t posTmp = LCD_current_position;
    setPosition(pos);
    LCD_changeNumber(pUpDown);
    setPosition(posTmp);
}

void LCD_OutTwoNumbers(uint8_t intNumber){
    if(((intNumber/10)<1)){LCD_INT(0);}
    LCD_INT(intNumber);
}