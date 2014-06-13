#ifndef __COMMANDS_H__
#define __COMMANDS_H__


#include <stdint.h>

void saveField(field *pField);
void go2nextField(uint8_t pDirection);
void go2nextFieldfree(uint8_t pDirection);
void go2Layer(uint8_t LayerId);
void go2Field(uint8_t Pos);
void changeText(uint8_t pDirection);
void sendTextToNext(uint8_t null);
#endif
