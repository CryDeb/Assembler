#include "layercontent.h"
#include "display.h"
#include "menu.h"

extern settings AppSettings;
extern uint8_t HH, MM, SS;
void updateLayerEditText(uint8_t pStatus){
    uint8_t x = AppSettings.currentField->Pos.PosX;
    uint8_t y = AppSettings.currentField->Pos.PosY;
    if(pStatus == 7){
	setPosition_XY(15,0);
	LCD_STR("^");
	setPosition_XY(15,2);
	LCD_STR("v");
	setPosition_XY(0,3);
	LCD_STR("<  >SEND-TEXT  >");
	setPosition_XY(x,y);
    }else{
	if(0x04&pStatus){//Cursor
	    setPosition_XY(x,y);
	}
	if(0x02&pStatus){//Statc
	}
	if(0x01&pStatus){//Dinamic
	}
    }
}