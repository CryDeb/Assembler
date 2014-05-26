#ifndef __INPUT_H__
#define __INPUT_H__

#include <avr/io.h>
#include <stdint.h>
// 4Mhz f�r delay
#define F_CPU 3686400UL  
#include <util/delay.h> 


#if !defined(KEYS) || !defined(ROW) || ! defined(COLUMN)
// data direction port a
#define KEYS	DDRB
// port a output
#define ROW 	PORTB
//port a input
#define COLUMN	PINB
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
