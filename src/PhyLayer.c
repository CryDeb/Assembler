#include "PhyLayer.h"
#include <avr/interrupt.h>
// todo to protocol.c
#define F_CPU 3686400UL     /* Quarz mit 3.6864 Mhz */

void initPhyLayer() {
	// set to input
	PhyPort_D = ~PhyPin;
	// set pull up
	PhyPortTX = ~PhyPin;
	//todo refactor define for int
	GICR = 1<<INT0;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
 
	//sei();
}
void putByte(uint8_t Byte) {
	uint8_t Bit = 0;
	uint8_t Count;
	// STOP Byte START
	uint16_t Frame = 0;
	Frame |= (1 << 9) | (Byte << 1);
	//_delay_ms(TIMEBETWEENBITS);
	for(Count = 10; Count > 0; Count--) {
		Bit = Frame & 0x01;
		PhyPortTX = Bit << PhyPin;
		_delay_ms(TIMEBETWEENBITS);
		if((PhyPortTX & PhyPin) == Bit) {
			break;
		}
		Frame >>= 1;
	}
	if((PhyPortTX & PhyPin) != Bit || Count > 0) {
		sendJam();
	}
}
uint8_t getByte() {
	uint8_t Count;
	uint8_t Byte = 0;
	_delay_ms(TIMEBETWEENBITS);
	for(Count = 9; Count > 0; Count--) {
		if(Count > 1) {
			if(PhyPortRX & PhyPin) {
				Byte |= 128;
			}
			Byte >>= 1;
		}
		_delay_ms(TIMEBETWEENBITS);
	}
	return Byte;
}

void sendJam() {
	uint8_t Count;
	for(Count = 8; (Count > 0); Count--) {
		PhyPortTX &= ~PhyPin;
		_delay_ms(TIMEBETWEENBITS);
	}
}

ISR(INT0_vect) {
	Byte = getByte();
}
