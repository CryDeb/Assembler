#ifndef __PHYLAYER_H__
#define __PHYLAYER_H__

#include <stdint.h>
#include <util/delay.h>   
// default suart ports
#if !defined(PhyPortRX) || !defined(PhyPortTX) || !defined(PhyPort_D) || !defined(PhyPin)
#define PhyPortRX	PIND
#define PhyPortTX	PORTD
#define PhyPort_D	DDRD
#define PhyPin		PD3
#endif
#define INTPin		INT0
#define INTMode		1<<ISC01;
// Default Baudrate
#ifndef BUADRATE
#define BAUDRATE 9600
#endif

#define TIMEBETWEENBITS 1000

typedef enum { RX, TX} PHYModes;

void initPhyLayer();
uint8_t putByte(uint8_t Byte);
uint8_t getByte();
void sendJam();
void putString(char *Buffer);
void setMode(PHYModes Mode);

#endif
