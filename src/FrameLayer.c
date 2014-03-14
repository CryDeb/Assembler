#include <avr/io.h>
#include <stdint.h>
#include "FrameLayer.h"
#include "PhyLayer.h"

extern frame_t LastFrame;

typedef enum {
	Destination,
	Source,
	SeqNumL,
	SeqNumH,
	CommandNum,
	Parameters,
	// insert new elem here
	NumFrameElements, // special length element
	CheckSum
} RecvState_t;

uint8_t sendFrame(frame_t Frame) {
	uint8_t Counter;
	uint8_t BCC = 0;
	char *IterPtr = (char *)&Frame;
	for(Counter = 0; Counter < NumFrameElements; Counter++, IterPtr++) {
		putByte(*IterPtr);
		if(Counter == Parameters) {
			// get num parameters
			uint8_t ParamLength = *IterPtr;
			for(;ParamLength > 0; ParamLength--, IterPtr++) {
				putByte(*IterPtr);
				BCC ^= *IterPtr;
			}
		}
		BCC ^= *IterPtr; 
	}
	putByte(BCC);
}

frame_t receiveFrame() {
	// todo timeout blocking ?
	setMode(RX);
}

uint8_t receive(uint8_t Destination) {
	char MoreToRecv = 1;
	uint8_t Byte;
	uint8_t BCC = 0;
	RecvState_t CurrentState = Source;
	int8_t ParameterLength = -1;
	// todo equ address
	if(Destination == 1) {
		while(MoreToRecv) {
			Byte = getByte();
			switch(CurrentState) {
				case Source:
					LastFrame.Source = Byte;
					break;
				case SeqNumL:
					LastFrame.SequenceNumL = Byte;
					break;
				case SeqNumH:
					LastFrame.SequenceNumH = Byte;
					break;
				case CommandNum:
					LastFrame.CommandNumber = Byte;
					break;
				case Parameters:
					if(ParameterLength = -1) {
						ParameterLength = Byte;
						// malloc length byte array
					} else {
						// add to paramter array
						if(--ParameterLength) {
							CurrentState = CheckSum;
						}
					}
					break;
				case CheckSum:
					if(BCC == Byte) {
						// ack
					} else {
						// nak
					}
			}
			BCC ^= Byte;
		}
	}
}
