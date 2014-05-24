#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdint.h>
#include <stddef.h>

typedef enum {
	INVALIDRBREF = -100,
	BUFFEREMPTY,
	OVERFLOW,
} RB_Errors;

typedef struct {
	uint8_t *Data;
	uint8_t Size;
	uint8_t Sentinel;
	uint8_t CurrentEntry;
	uint8_t Counter;
} RingBuffer;

RingBuffer *createRingBuffer(uint8_t Size);
void addEntry(RingBuffer *Buffer, int16_t Byte);
int16_t getEntry(RingBuffer *Buffer); 
void ReleaseRingBuffer(RingBuffer *Buffer);
void clearRingBuffer(RingBuffer *Buffer);

#endif
