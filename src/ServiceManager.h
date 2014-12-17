#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#include <stdint.h>
#include "FrameLayer.h"
//anzahl services
#define MAX_SERVICES 10

typedef void (*remoteJob)(frame_t *);

extern remoteJob Services[MAX_SERVICES];

void addService(uint8_t CommandNr, remoteJob Job);
void removeService(uint8_t CommandNr);

#endif
