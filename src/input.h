#ifndef __INPUT_H__
#define __INPUT_H__

#include <avr/io.h>
#include <stdint.h>
// 4Mhz für delay
#define F_CPU 3686400UL  
#include <util/delay.h> 


#if !defined(KEYS) || !defined(ROW) || ! defined(COLUMN)
// data direction port a
#define KEYS	DDRA
// port a output
#define ROW 	PORTA
//port a input
#define COLUMN	PINA
#endif

// low nibble
#define LNIB	0x0F
// high nibble
#define HNIB	0xF0

// default 4 reihen
#ifndef NUMROWS
// anzahl tasten in reihe
#define NUMROWS	4
#endif 

uint8_t scanCode();
uint8_t bit_pos(uint8_t Value);

#endif
