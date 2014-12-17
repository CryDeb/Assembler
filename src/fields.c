#include "fields.h"

/*******************************************************************************
* Beschreibung:
*	Hier sind alle Felder definiert. Jedes Feld kann einem oder mehereren 
*	Layern hinzugef�gt werden.
*******************************************************************************/

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




// ShowTimeLayer
field goToLeftLayerTime = {
	.Pos = {0, 3},
	.Enter = {&go2Layer, 1},
	.Right = {&go2Field, (3<<6)|(5)}
};
field goToRightLayerTime = {
        .Pos = {15, 3},
	.Left = {&go2Field, ((3<<6)|(5))},
	.Enter = {&go2Layer, 1}
};
field goToEdit = {
	.Pos = {5, 3},
	.Left = {&go2Field, ((3<<6)|(0))},
	.Enter = {&go2Layer, 3},
	.Right = {&go2Field, ((3<<6)|(15))}
};

//ChangeTimeLayer
field changeHour = {
	.Pos = {1, 1},
	.Left = {&go2Field, ((11)|(1<<6))},
	.Right = {&go2Field, ((6)|(1<<6))},
	//.Enter = {&changeNumberDown, 1},
	.Up = {&changeNumber, 1},
	.Down = {&go2Field, ((2)|(3<<6))}
};
field changeMinute = {
	.Pos = {6, 1},
	.Left = {&go2Field, ((1)|(1<<6))},
	.Right = {&go2Field, ((11)|(1<<6))},
	.Down = {&go2Field, ((2)|(3<<6))},
	.Up = {&changeNumber, 2}
};
field changeSecond = {
	.Pos = {11, 1},
	.Left = {&go2Field, ((6)|(1<<6))},
	.Right = {&go2Field, ((1)|(1<<6))},
	.Down = {&go2Field, ((9)|(3<<6))},
	.Up = {&changeNumber, 3}
};
field safeChange = {
	.Pos = {2, 3},
	.Up =  {&go2Field, ((1)|(1<<6))},
	.Left = {&go2Field, ((9)|(3<<6))},
	.Right = {&go2Field, ((9)|(3<<6))}
};
field discardChange = {
	.Pos = {9, 3},
	.Up =  {&go2Field, ((11)|(1<<6))},
	.Left = {&go2Field, ((2)|(3<<6))},
	.Right = {&go2Field, ((2)|(3<<6))},
	.Enter = {&go2Layer, 2}
};


field goToLeftLayerSettings = {
	.Pos = {0,3},
	.Enter = {&go2Layer, 2},
	.Left = {&go2Field, ((15)|(3<<6))},
	.Right = {&go2Field, ((15)|(3<<6))}
};
field goToRightLayerSettings = {
	.Pos = {15,3},
	.Enter = {&go2Layer, 2},
	.Left = {&go2Field, ((0)|(3<<6))},
	.Right = {&go2Field, ((0)|(3<<6))}
};
