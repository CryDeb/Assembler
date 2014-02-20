#ifndef __PHYLAYER_H__
#define __PHYLAYER_H__

#include <stdint.h>
#include <util/delay.h>   
// default suart ports
#if !defined(PhyPort) || !defined(PhyPort) || !defined(PhyPort_D) || !defined(PhyPin)
#define PhyPortRX	PIND
#define PhyPortTX	PORTD
#define PhyPort_D	DDRD
#define PhyPin		PD2
#endif
// Default Baudrate
#ifndef BUADRATE
#define BAUDRATE 9600
#endif

#define TIMEBETWEENBITS 1

typedef enum { RX, TX} PHYModes;

uint8_t Byte;

void initPhyLayer();
void putByte(uint8_t Byte);
uint8_t getByte();
void sendJam();


#endif
