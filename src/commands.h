#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdint.h>

void go2Layer(uint8_t LayerId);
void go2Field(uint8_t Pos);
void changeNumber(uint8_t changeValue);
void changeTimeNumber(uint8_t *Time, uint8_t maxVal);

#endif
