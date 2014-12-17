#ifndef __FRAMELAYER_H__
#define __FRAMELAYER_H__
// frame struktur
typedef struct {
	uint8_t Destination;
	uint8_t Source;
	uint8_t SequenceNumL;
	uint8_t SequenceNumH;
	uint8_t	CommandNumber;
	char *Parameters; __attribute__((packed))
	uint8_t CheckSum;
} frame_t;

//states für statemachine (einlesen eines frames)
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

/* prototypes */
uint8_t sendFrame(frame_t *Packet);
void receiveFrame(uint8_t Byte);
uint8_t receive(uint8_t Destination);
void handleIncomingTask();
void initLayer();

#endif
