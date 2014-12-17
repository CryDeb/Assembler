#include "layercontent.h"
#include "display.h"
#include "menu.h"
// menu settings & variablen
extern settings AppSettings;
// Stunden, Minuten, Sekunde
extern uint8_t HH, MM, SS;

/*******************************************************************************
* Beschreibung:
*	Hier werden die Inhalte der Ebenen ausgegeben. Alle Ebenen haben einen 
*	handler der die Ausgabe verwaltet, diese werden hier definiert.
*******************************************************************************/

void updateLayerShowTime(uint8_t pStatus){
    uint8_t x = AppSettings.currentField->Pos.PosX;
    uint8_t y = AppSettings.currentField->Pos.PosY;
    if(pStatus == 7){
	setPosition_XY(4,1);
	LCD_OutTwoNumbers(HH);
	LCD_STR(":");
	LCD_OutTwoNumbers(MM);
	LCD_STR(":");
	LCD_OutTwoNumbers(SS);
	setPosition_XY(0,3);
	LCD_STR("<");
	setPosition_XY(5,3);
	LCD_STR(">Edit");
	setPosition_XY(15,3);
	LCD_STR(">");
	setPosition_XY(x,y);
    }else{
	if(0x04&pStatus){//Cursor
	    setPosition_XY(x,y);
	}
	if(0x02&pStatus){//Static

	}
	if(0x01&pStatus){//Dinamic

	}
    }
}
void updateLayerEditTime(uint8_t pStatus){
    uint8_t x = AppSettings.currentField->Pos.PosX;
    uint8_t y = AppSettings.currentField->Pos.PosY;
    if(pStatus == 7){
	setPosition_XY(1, 1);
	LCD_STR(">");
	LCD_OutTwoNumbers(HH);
	LCD_STR("h >");
	LCD_OutTwoNumbers(MM);
	LCD_STR("m >");
	LCD_OutTwoNumbers(SS);
	LCD_STR("s");
	setPosition_XY(2, 3);
	LCD_STR(">Safe  >Back");
	setPosition_XY(x,y);
    }else{
	if(0x04&pStatus){//Cursor
	    setPosition_XY(x,y);
	}
	if(0x02&pStatus){//Static

	}
	if(0x01&pStatus){//Dinamic
	}
    }
}

void updateLayerSettings(uint8_t pStatus){
    uint8_t x = AppSettings.currentField->Pos.PosX;
    uint8_t y = AppSettings.currentField->Pos.PosY;
    if(pStatus == 7){
	setPosition_XY(0, 0);
	LCD_STR("Adresse:______  Geraet :______");
	setPosition_XY(0, 3);
	LCD_STR("<");
	setPosition_XY(15, 3);
	LCD_STR(">");
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
