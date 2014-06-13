#include <avr/io.h>
#include <stdint.h>
#include "FrameLayer.h"
#include "PhyLayer.h"

frame_t LastFrame;
frame_t PendingFrame;
uint8_t Pending;
RecvState_t CurrentState;
// todo relocate 
// temporary proof of concept 
void printAToPortC(frame_t *Frame) {
	PORTC = 'A';
}




/************* TODO:Dane Wicki **********************/



remoteJob Commands[10] = {0,0,0,0,printAToPortC,0};

uint8_t sendFrame(frame_t *Frame) {
	char Counter;
	uint8_t BCC = 0;
	char *IterPtr = (char *)Frame;
	uint8_t Elements = NumFrameElements - 1;
	for(Counter = 0; Counter < Elements; Counter++, IterPtr++) { 
		if(Counter == Parameters) {
			//TODO fix 
			IterPtr = *IterPtr;
			// get number of parameters 
			Elements += *IterPtr;
		}
		putByte(*IterPtr);
		BCC ^= *IterPtr;
	}
	putByte(BCC);
	if(getByte(TRUE) == NAK) {
		//TODO
	}
}

void receiveFrame(uint8_t Byte) {
	switch(CurrentState) {
		case Destination:
			//TODO 
			/*if(Byte != LocalAddr) {
				ignore frame
			} else if (Pending == TRUE) {
				// the else if got evaluated if destination address
				// is local address
				drop this frame 
			}*/
			LastFrame.CheckSum = 0;
			LastFrame.Destination = Byte;
			CurrentState = Source;
		case Source:
			LastFrame.Source = Byte;
			CurrentState = SeqNumL;
			break;
		case SeqNumL:
			LastFrame.SequenceNumL = Byte;
			CurrentState = SeqNumH;
			break;
		case SeqNumH:
			LastFrame.SequenceNumH = Byte;
			CurrentState = CommandNum;
			break;
		case CommandNum:
			LastFrame.CommandNumber = Byte;
			CurrentState = Parameters;
			break;
		case Parameters:
			//TODO 
			CurrentState = CheckSum;
			break;
		case CheckSum:
			if(LastFrame.CheckSum == Byte) {
				putByteAsync(ACK, TRUE);
				memcpy(&PendingFrame,&LastFrame, sizeof(frame_t));
				Pending = TRUE;
			} else {
				putByteAsync(NAK, TRUE);
			}
	}
	LastFrame.CheckSum ^= Byte;
}

void handleIncomingTask() {
	if(Pending) {
		(*Commands[PendingFrame.CommandNumber])(&PendingFrame);
	}
}
