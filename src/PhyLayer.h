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

#define ACK 0x06
#define NAK 0x15

#define TRUE 1
#define FALSE 0

#define BUFFERSIZE 3

typedef enum { RX, TX} PHYModes;
typedef enum { CS_STARTBIT, CS_FRAMEBIT, CS_STOPBIT, CS_SENDJAM,CS_FINISHED} ComunicationState;
typedef enum { 
	SUCCESS = 0, 
	COLLISIONDETECTED, 
	BUFFEROVF, 
	INVALIDSTATE,
	WOULDBLOCK
} CommunicationError;

typedef void (*ByteReceivedHandler)(uint8_t Byte);

ByteReceivedHandler receivedByte;
volatile PHYModes CommunicationMode;
volatile ComunicationState CurrentState;
volatile uint8_t Frame;
volatile CommunicationError LastError;

CommunicationError initPhyLayer();
CommunicationError putByte(uint8_t Byte);
CommunicationError getByte(uint8_t *Byte);
CommunicationError putByteAsync(uint8_t Byte, uint8_t Block);
CommunicationError getByteAsync(uint8_t Block);
CommunicationError putString(char *Buffer);
CommunicationError setMode(PHYModes Mode);
CommunicationError getLastError();
CommunicationError startListening(ByteReceivedHandler Handler);
CommunicationError stopListening();


#endif
