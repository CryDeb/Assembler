#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdint.h>
#include <stddef.h>
// error codes
typedef enum {
	INVALIDRBREF = -100,
	BUFFEREMPTY,
	OVERFLOW,
} RB_Errors;

//ringbuffer enthält die daten und steurvariablen
typedef struct {
	// zeiger auf daten
	uint8_t *Data;
	// anzahl möglicher daten
	uint8_t Size;
	// offset des sentinels
	// der nächste offset
	uint8_t Sentinel;
	// offset des aktuellen bytes
	uint8_t CurrentEntry;
	// anzahl von werten im buffer
	uint8_t Counter;
} RingBuffer;

/* prototypes */
RingBuffer *createRingBuffer(uint8_t Size);
int16_t addEntry(RingBuffer *Buffer, uint8_t Byte);
int16_t getEntry(RingBuffer *Buffer); 
void releaseRingBuffer(RingBuffer *Buffer);
void clearRingBuffer(RingBuffer *Buffer);

#endif
