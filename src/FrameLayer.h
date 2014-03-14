#ifndef __FRAMELAYER_H__
#define __FRAMELAYER_H__

typedef struct {
	uint8_t Destination;
	uint8_t Source;
	uint8_t SequenceNumL;
	uint8_t SequenceNumH;
	uint8_t	CommandNumber;
	char *Parameters;
} frame_t;

frame_t LastFrame;

uint8_t sendFrame(frame_t Packet);
frame_t receiveFrame();
uint8_t receive(uint8_t Destination);

#endif
