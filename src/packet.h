#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>

typedef struct {
	
} parameters;

typedef struct {
	uint8_t Source;
	uint8_t Destination;
	//seq
	uint8_t Command
	parameters Params;
	uint8_t BCC;	
} packet;

#endif
