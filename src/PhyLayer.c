#include "PhyLayer.h"
#include <avr/interrupt.h>
// todo to protocol.c
#define F_CPU 3686400UL     /* Quarz mit 3.6864 Mhz */
#define PIN (1 << (PhyPin - 1))

void initPhyLayer() {
	setMode(RX);
}
uint8_t putByte(uint8_t Byte) {
	uint8_t Error = 0;
	uint8_t Bit = 0;
	uint8_t Count;
	uint8_t Pin = PhyPin - 1;
	// STOP Byte START
	uint16_t Frame = 0;
	Frame |= (Byte << 1);
	for(Count = 9; Count > 0; Count--) {
		Bit = (Frame & 0x01);
		//refactor 
		PhyPortTX = (Bit) ? (1<<Pin) : ~(1<<Pin);
		// ~(PhyPortTX & (1 << Pin));
		W1s();//_delay_ms(TIMEBETWEENBITS);
		/*if((PhyPortTX & PhyPin) == ~Bit) {
			break;
		}*/
		Frame >>= 1;
	}
	/*if((PhyPortTX & PhyPin) != ~Bit || Count > 0) {
		// todo error code
		Error = 1;
		sendJam();
	}*/
	PhyPortTX |= 1 << Pin;
	return Error;
}

void putString(char *Buffer) {
	setMode(TX);
	while(*Buffer != 0 && putByte(*Buffer))
		Buffer++;
}

uint8_t getByte() {
	int8_t Count;
	uint8_t Byte = 0;
	//_delay_ms(TIMEBETWEENBITS + TIMEBETWEENBITS / 2 );
	W1s();W100ms();W100ms();W100ms();W100ms();
	//W100ms();
	for(Count = 0; Count <= 7; Count++) {
		Byte |= ((~(PhyPortTX & PIN)) << Count);
		W1s();//_delay_ms(TIMEBETWEENBITS);
	}
	return Byte;
}

void setMode(PHYModes Mode) {
	uint8_t PinNum = PhyPin-1;
	if(Mode == RX) {
		// set to input
		PhyPort_D &= ~(1 << PinNum);
		// set pull up
		PhyPortTX |= (1 << PinNum);
			//todo refactor define for int
		GICR = 1<<INTPin;					// Enable INT0
		MCUCR = INTMode	// Trigger INT0 on falling edge
 
		sei();
	} else {
		PhyPort_D |= (1 << PinNum);
		GICR &= ~(1<<INTPin);
	}
}


void sendJam() {
	uint8_t Count;
	for(Count = 8; (Count > 0); Count--) {
		PhyPortTX &= ~(1<<PIN);
		_delay_ms(TIMEBETWEENBITS);
	}
}

ISR(INT0_vect) {
	LCD_STR(".");
	uint8_t Byte = getByte();
	PORTC = ~Byte;
	//receive(Byte);
	//PORTC = Byte;
	cli();
	sei();
}
