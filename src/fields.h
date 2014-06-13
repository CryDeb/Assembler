#ifndef __FIELDS_H__
#define __FIELDS_H__

#include "menu.h"
#include "commands.h"

/*
 *                LCD_POSITIONS
 * 
 *   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
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
#


field editText000 = {
    .Pos = {0, 0},
    //NULL, Up,
    //NULL, Left,
    //Null, Enter,
    .Right = {&go2Field, (0<<6)|(1)},
    .Down = {&go2Field, (0<<6)|(3)}
};
// ShowTimeLayer
field goToLeftLayerTime = {
	.Pos = {0, 3},
	//NULL,UP
	//NULL,	Left
	.Enter = {&go2Layer, 1},//Enter
	.Right = {&go2Field, (3<<6)|(5)} //Right
	//NULL,  Down
};
field goToRightLayerTime = {
        .Pos = {15, 3},
	//NULL,
	.Left = {&go2Field, ((3<<6)|(5))},
	.Enter = {&go2Layer, 1}
	//NULL,
	//NULL
};
field goToEdit = {
	.Pos = {5, 3},
	//NULL,
	.Left = {&go2Field, ((3<<6)|(0))},
	.Enter = {&go2Layer, 3},
	.Right = {&go2Field, ((3<<6)|(15))}
	//NULL
};

//ChangeTimeLayer
field changeHour = {
	.Pos = {1, 1},
	.Left = {&go2Field, ((11)|(1<<6))},
	.Right = {&go2Field, ((6)|(1<<6))},
	//.Enter = {&changeNumberDown, 1},
	.Up = {&changeNumber, 1}, //, PLACEHOLDER_LEFT, PLACEHOLDER_ENTER, PLACEHOLDER_RIGHT
	.Down = {&go2Field, ((2)|(3<<6))}
};
field changeMinute = {
	.Pos = {6, 1},
	.Left = {&go2Field, ((1)|(1<<6))},
	.Right = {&go2Field, ((11)|(1<<6))},
	.Down = {&go2Field, ((2)|(3<<6))},
	.Up = {&changeNumber, 2}
	//, PLACEHOLDER_UP, PLACEHOLDER_LEFT, PLACEHOLDER_ENTER, PLACEHOLDER_RIGHT, PLACEHOLDER_DOWN
};
field changeSecond = {
	.Pos = {11, 1},
	.Left = {&go2Field, ((6)|(1<<6))},
	.Right = {&go2Field, ((1)|(1<<6))},
	.Down = {&go2Field, ((9)|(3<<6))},
	.Up = {&changeNumber, 3}
	//{11, 1}, PLACEHOLDER_UP, PLACEHOLDER_LEFT, PLACEHOLDER_ENTER, PLACEHOLDER_RIGHT, PLACEHOLDER_DOWN
};
field safeChange = {
	.Pos = {2, 3},
	.Up =  {&go2Field, ((1)|(1<<6))},
	.Left = {&go2Field, ((9)|(3<<6))},
	.Right = {&go2Field, ((9)|(3<<6))}
	//{3, 3}, NULL, PLACEHOLDER_LEFT, PLACEHOLDER_ENTER, PLACEHOLDER_RIGHT, NULL
};
field discardChange = {
	.Pos = {9, 3},
	.Up =  {&go2Field, ((11)|(1<<6))},
	.Left = {&go2Field, ((2)|(3<<6))},
	.Right = {&go2Field, ((2)|(3<<6))},
	.Enter = {&go2Layer, 2}
	//{9, 3}, NULL, PLACEHOLDER_LEFT, PLACEHOLDER_ENTER, PLACEHOLDER_RIGHT, NULL
};


field goToLeftLayerSettings = {
	.Pos = {0,3},
	.Enter = {&go2Layer, 2},
	.Left = {&go2Field, ((15)|(3<<6))},
	.Right = {&go2Field, ((15)|(3<<6))}
	//{0, 3}, NULL, NULL, PLACEHOLDER_ENTER, go2Field((pos)), NULL
};
field goToRightLayerSettings = {
	.Pos = {15,3},
	.Enter = {&go2Layer, 2},
	.Left = {&go2Field, ((0)|(3<<6))},
	.Right = {&go2Field, ((0)|(3<<6))}
    //{15, 3},NULL,PLACEHOLDER_LEFT,PLACEHOLDER_ENTER,NULL,NULL
};

#endif