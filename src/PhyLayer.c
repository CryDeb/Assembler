#include "PhyLayer.h"
#include "ringbuffer.h"
#include <avr/interrupt.h>
#define PIN (1 << (PhyPin - 1))

volatile ByteReceivedHandler receivedByte = NULL;

volatile PHYModes CommunicationMode;
volatile ComunicationState CurrentState;
volatile CommunicationError LastError;
volatile uint8_t Frame;
uint8_t NumBitCounter;
RingBuffer *Buffer;

#define NOP asm volatile ("nop")
#define SET_LOW(x) ((x) &= ~PIN)
#define SET_HIGH(x) ((x) |= PIN)

CommunicationError initPhyLayer() {
	CurrentState = CS_FINISHED;
	//set timer to ctc mode
	OCR0 = 2;
	TCCR0 = 0x00;
	TCCR0 = (1<<WGM01); 
	// set prescaler
	TCCR0 = (1<<CS00); 
	Buffer = createRingBuffer(BUFFERSIZE);
	return SUCCESS;
}

CommunicationError setMode(PHYModes Mode) {
	CommunicationError Error = SUCCESS;
	if(CurrentState == CS_FINISHED)
	{
		TCCR0 = /*(1<<CS02) |*/ (1<<CS00);
		CommunicationMode = Mode;
		if(Mode == RX) {
			// set to input
			PhyPort_D &= ~(PIN);
			// set pull up
			PhyPortTX |= PIN;
			// Trigger INT on falling edge
			MCUCR = INTMode;

			PhyPort_D |= 8;
			MCUCR |= 1 << ISC11;
					
		} else {
			// set to output
			PhyPort_D |= PIN;
			// disable INT0
			GICR &= ~(1<<INTPin);
		}
	} else {
		Error = INVALIDSTATE;
	}
	return LastError = Error;
}

CommunicationError getLastError() {
	return LastError;
}

CommunicationError putByte(uint8_t Byte) {
	CommunicationError Error = SUCCESS;

	putByteAsync(Byte, TRUE);
	while(CurrentState != CS_FINISHED) NOP;

	return LastError = Error;
	
}

CommunicationError getByte(uint8_t *Byte) {
	CommunicationError Error = SUCCESS;
	int16_t Data = getEntry(Buffer);
	if(Data == BUFFEREMPTY) {
		// start async recv and wait
		Error = getByteAsync(TRUE);
		while(CurrentState != CS_FINISHED) NOP;
	}
	Data = getEntry(Buffer);
	if(Data > 0) {
		Byte = (uint8_t)Data;
	}

	return LastError = Error;
}

CommunicationError putString(char *Buffer) {
	CommunicationError Error = SUCCESS;
	setMode(TX);
	while(*Buffer != 0 && (putByte(*Buffer) == SUCCESS))
		Buffer++;
	return LastError = Error;
}

CommunicationError putByteAsync(uint8_t Byte, uint8_t Block) {
	CommunicationError Error = SUCCESS;
	if(CurrentState != CS_FINISHED && Block) {
		while(CurrentState != CS_FINISHED) NOP;
	} else {
		Error == WOULDBLOCK;
	}
	if(Error == SUCCESS) {
		setMode(TX);
		PORTC = ~Byte;
		Frame = Byte;
		CurrentState = CS_STARTBIT;
		TCCR0 =  (1<<CS00);
	  //enable timer
	  TIMSK	|= (1<<OCIE0);
		sei();
	}

	return LastError = Error;
}

CommunicationError getByteAsync(uint8_t Block) {
	CommunicationError Error = SUCCESS;
	/*if(CurrentState != CS_FINISHED && Block) {*/
		
		while(CurrentState != CS_FINISHED) NOP;
		setMode(RX);
		if(CommunicationMode == RX)
		{
			// clear INT0
			GIFR |= (1<<INTF0);
			// Enable INT0
			GICR |= 1<<INT0;
			sei();
		}
	/*} else {
		Error == WOULDBLOCK;
	}*/
	
	return LastError = Error;
}

//set listen mode to listen for all bytes
//set event listener for int1
CommunicationError startListening(ByteReceivedHandler Handler) {
	if(Handler != NULL) {
		receivedByte = Handler;
		// clear INT1
		GIFR |= (1<<INTF1);
		// Enable INT1
		GICR |= 1<<INT1;
		setMode(RX);
		// clear INT0
		GIFR |= (1<<INTF0);
		// Enable INT0
		GICR |= 1<<INT0;
		sei();
	}
}
CommunicationError stopListening() {
	GICR &= ~(1<<INT1);	//disable INT0
	receivedByte = NULL;
	return SUCCESS;
}

ISR(INT1_vect) {
	//execute function pointer
	//event listener
	int16_t	Data = getEntry(Buffer);

	if(Data > 0) {
		
		receivedByte((uint8_t)Data);
	}
}

ISR(INT0_vect) {
	CurrentState = CS_STARTBIT;
	OCR0 = 127;
	TCCR0 = (1<<CS00);
	TIMSK	|= (1<<OCIE0);
	GICR &= ~(1<<INTPin);	//disable INT0 
}

ISR(TIMER0_COMP_vect)
//ISR(TIMER0_OVF_vect)
{
	uint8_t Sent = 0;
	switch(CommunicationMode) {
		case TX:
			// send value in Frame
			switch(CurrentState) {
				case CS_STARTBIT:
					NumBitCounter = 0;
					// set to low to indicate start
					//PhyPortTX &= Sent = ~PIN;
					Sent = SET_LOW(PhyPortTX);
					CurrentState = CS_FRAMEBIT;
					break;
				case CS_FRAMEBIT:
					// set lsb of frame 
					Sent = (Frame & 0x01) ? SET_HIGH(PhyPortTX) : SET_LOW(PhyPortTX);
					// new lsb
					Frame >>= 1;
					if(++NumBitCounter >= 8)
						CurrentState = CS_STOPBIT;
					break;
				case CS_SENDJAM:
					// set the neg of current value
					//PhyPortTX &= ~PIN;//(PhyPortTX & PIN) ? ~PIN : PIN;
					SET_LOW(PhyPortTX);
					// new lsb
					Frame >>= 1;
					if(++NumBitCounter >= 8)
						CurrentState = CS_FINISHED;
				case CS_STOPBIT:
					// set to higth to stop 
					//PhyPortTX |= Sent = PIN;
					Sent = SET_HIGH(PhyPortTX);
					CurrentState = CS_FINISHED;
					
					break;
				case CS_FINISHED:
				default:
					// stop timer
					// TIMSK &= ~(1<<TOIE0);
					TCCR0 = 0;
					break;
					
			}
			/*if(CurrentState != CS_FINISHED && CurrentState != CS_SENDJAM && PhyPortTX != Sent) {
				CurrentState = CS_SENDJAM;
				NumBitCounter = 0;
			}*/
			break;
		case RX:
			switch(CurrentState) {
				case CS_STARTBIT:
					// we're in the middl of the start bit
					Frame = 0;
					NumBitCounter = 0;
					CurrentState = CS_FRAMEBIT;
					// reset counter to full length
					OCR0 = 254;
					TCNT0 = 0;
					break;
				case CS_FRAMEBIT:
					// receive lsb to msb
					Frame |= (((~PhyPortRX) & PIN)) ? 0 : 1 << NumBitCounter;
					
					if(++NumBitCounter > 7)
					{
						CurrentState = CS_STOPBIT;
						
					}
					break;
				case CS_STOPBIT:
					CurrentState = CS_FINISHED;
					uint8_t Bit = (((~PhyPortRX) & PIN)) ? 0 : 1 << NumBitCounter;
					if(Bit != 1)
					{
						//Jam recv
						LastError = COLLISIONDETECTED;
					}
					break;
				case CS_FINISHED:
				default:
					
					addEntry(Buffer, Frame);
					//todo error check
					// set prescaler to 0 
					// to stop the timer
					GIFR |= (1<<INTF0);
					TCCR0 = 0;
		
					if(receivedByte != NULL) {
						PhyPortTX |= 8;
						PhyPortTX &= ~8;
						// clear INT0
						GIFR |= (1<<INTF0);
						// Enable INT0
						GICR |= 1<<INT0;
					}
					break;	
			}
			break;
	}
	
}



