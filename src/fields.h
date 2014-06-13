#ifndef __FIELDS_H__
#define __FIELDS_H__

#include "menu.h"
#include "commands.h"

/*
 *                LCD_POSITIONS
 * 
 *   01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
 * 0  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
 * 1  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
 * 2  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
 * 3  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
 * 
 * Binary:
 *    Y-Y-Y X-X-X-X-X
 *      4       16
 *
 *  Aufbau fields:  Position, UpButton, LeftButton,
 *                  EnterButton, RightButton, DownButton
 * 
 *  
 */


field editText = {
	.Pos = {0, 0},
	.Left = {&go2nextFieldfree, _LEFT_},
	.Right = {&go2nextFieldfree, _RIGHT_},
	.Down = {&go2nextFieldfree, _DOWN_},
	.Up = {&go2nextFieldfree, _UP_},
        .AbcDown = {&changeText, _DOWN_},
        .AbcUp = {&changeText, _UP_}
	//, PLACEHOLDER_UP, PLACEHOLDER_LEFT, PLACEHOLDER_ENTER, PLACEHOLDER_RIGHT, PLACEHOLDER_DOWN
};
field sendingText = {
	.Pos = {3,3},
	.Left = {&go2nextField, _LEFT_},
	.Right = {&go2nextField, _RIGHT_},
	.Down = {&go2nextField, _DOWN_},
	.Up = {&go2nextField, _UP_},
	.Enter = {&sendTextToNext, 0}
};
field goToLeftLayer = {
	.Pos = {0,3},
	.Left = {&go2nextField, _LEFT_},
	.Right = {&go2nextField, _RIGHT_},
	.Down = {&go2nextField, _DOWN_},
	.Up = {&go2nextField, _UP_},
	.Enter = {&sendTextToNext, 0}
};
field goToRightLayer = {
	.Pos = {15,3},
	.Left = {&go2nextField, _LEFT_},
	.Right = {&go2nextField, _RIGHT_},
	.Down = {&go2nextField, _DOWN_},
	.Up = {&go2nextField, _UP_},
	.Enter = {&sendTextToNext, 0}
};

#endif