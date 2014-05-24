#include "ringbuffer.h"

RingBuffer *createRingBuffer(uint8_t Size) {
	RingBuffer *Buffer = (RingBuffer *)malloc(sizeof(RingBuffer));
	if(Buffer != NULL) {
		Buffer->Data =  (uint8_t*)malloc(sizeof(uint8_t)*Size);
		if(Buffer->Data != NULL) {
			clearRingBuffer(Buffer);
		} else {
			free(Buffer);
		}
	}
	
	return Buffer;
}

void clearRingBuffer(RingBuffer *Buffer) {
	Buffer->Sentinel = 0;
	Buffer->CurrentEntry = 0;
	Buffer->Counter = 0;
}

void ReleaseRingBuffer(RingBuffer *Buffer) {
	if(Buffer != NULL) {
		if(Buffer->Data != NULL)
			free(Buffer->Data);
		free(Buffer);
	}
}

void addEntry(RingBuffer *Buffer, int16_t Byte) {
	if(Buffer != NULL && Buffer->Data == NULL) {
		if(Buffer->Counter <= Buffer->Size) {
			*(Buffer->Data + Buffer->Sentinel) = Byte;
			Buffer->Sentinel = (Buffer->CurrentEntry + 1) % Buffer->Size;		
			Buffer->Counter++;
		} else {
				// overflow sentinal + 1 will be currententry index
				Byte = OVERFLOW;
		}
	}	
}

int16_t getEntry(RingBuffer *Buffer) {
	int16_t Byte = BUFFEREMPTY;
	if(Buffer != NULL && Buffer->Data != NULL) {
		if(Buffer->Counter > 0) {
			Byte = *(Buffer->Data + Buffer->CurrentEntry);
			Buffer->CurrentEntry++;
			Buffer->CurrentEntry %= Buffer->Size;
			Buffer->Counter--;
		} 
	} else {
			Byte == INVALIDRBREF;
	}
	return Byte;
}
