#include "layercontent.h"
#include "display.h"
#include "menu.h"
#include "commands.h"

extern settings AppSettings;
extern char SendingText[];

void updateLayerEditText(uint8_t pStatus){
	uint8_t x = AppSettings.currentField->Pos.PosX;
	uint8_t y = AppSettings.currentField->Pos.PosY;
	if(pStatus == 7){
		outputToLCD(0,0,15,3);
		
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
void updateRecivedText(uint8_t pStatus){
	uint8_t x = AppSettings.currentField->Pos.PosX;
	uint8_t y = AppSettings.currentField->Pos.PosY;
	if(pStatus == 7){
		outputToLCD(0,0,15,3);
		
		setPosition_XY(0,3);
		LCD_STR("<              >");
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

void outputToLCD(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY){
	for(uint8_t countY=startY;countY<endY;countY++){
		setPosition_XY(startX,countY);
		for(uint8_t countX=startX;countX<endX;countX++){
			LCD_CHR(SendingText[(countX + (countY*(endX-startX)))]);
		}
	}
}