#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#define _RIGHT_ 0
#define _LEFT_ 1
#define _DOWN_ 2
#define _UP_ 3
#define _DISPLAY_MAX_X 16
#define _DISPLAY_MAX_Y 4


#include <stdint.h>

void saveField(field *pField);
void go2nextField(uint8_t pDirection);
void go2nextFieldfree(uint8_t pDirection);
void go2Layer(uint8_t LayerId);
void go2Field(uint8_t Pos);
void changeText(uint8_t pDirection);
void sendTextToNext(uint8_t null);
#endif
