/* 
 * File:   delay.h
 * Author: dane
 *
 * Created on 12 May 2014, 08:20
 */

#ifndef DELAY_H
#define	DELAY_H
#ifdef __ASSEMBLER__
#define mpr	R16
#else
#include <stdint.h>
register uint8_t mpr asm("r16");

void W100us(void);
void W1ms(void);
void W10ms(void);
void W100ms(void);
void W1s(void);
void WXms(void);

#endif
#endif	/* DELAY_H */

