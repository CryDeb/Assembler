#include <avr/io.h>
#include <util/delay.h>
#include "input.h"



/**************************************************************************
*
* Funktion: 	Taste_einlesen
* Paramteter:	-
*
* Beschreibung: Liest aus einer Matrix-Tastatur den gedrueckten Scan-COde ein
* 
* Scan-Code:	
 *		01 05 09 13
 *		02 06 10 14
 *		03 07 11 15
 *		04 08 12 16
*
**************************************************************************/
uint8_t scanCode(){
	KEYS = LNIB;
	ROW = HNIB;
	
	uint8_t Count = 0;
	uint8_t Code = 0xFF;
	uint8_t mpr = 0;
	uint8_t mpr2 = 0;
	for(Count = 0; (Code == 0xFF) && (Count < NUMROWS); Count++) {
		ROW = ~(1 << Count);
		
		asm volatile ("nop");
		mpr = (~(COLUMN) >> 4) & LNIB;
		//entprellung
		_delay_ms(20);
		mpr2 = (~(COLUMN) >> 4) & LNIB;
		
	    if((mpr == mpr2)  && (mpr != 0)){
		Code = bit_pos(mpr) + Count * NUMROWS;
	    }
	}
	return Code;
}

uint8_t bit_pos(uint8_t Value){
	// position des bit
	uint8_t Position = 0;
	while(Value){
		Position++;
		Value >>= 1;
	}
	return Position;
}
