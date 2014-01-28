#include <avr/io.h>
#include <util/delay.h>
#include "input.h"



/**************************************************************************
*
* Funktion: 	Taste_einlesen
* Paramteter:	-
*
* Beschreibung: Liest aus einer Matrix-Tastatur den gedrückten Scan-COde ein
* 
* Scan-Code:	1  2  3  4
*				5  6  7  8
*				9 10 11 12
*			   13 14 15 16
*
**************************************************************************/
uint8_t scanCode(){
	KEYS = LNIB;
	ROW = HNIB;
	
	uint8_t Count = 0;
	uint8_t Code = 0xFF;
	uint8_t mpr = 0;
	uint8_t mpr2 = 0;
	static uint8_t Last = 0xFF;

	for(Count = 0; (Code == 0xFF) && (Count < NUMROWS); Count++) {
		ROW = ~(1 << Count);
		
		asm volatile ("nop");
		mpr = (~(COLUMN) >> 4) & LNIB;
		//entprellung
		_delay_ms(20);
		mpr2 = (~(COLUMN) >> 4) & LNIB;
		
		if((mpr == mpr2) && (mpr != 0) && (Last != mpr)) {
			Last = Code = bit_pos(mpr) + Count * NUMROWS;
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
