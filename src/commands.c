#include <stdint.h>
#include <stddef.h>
#include "menu.h"
#include "commands.h"
#include "display.h"

extern settings AppSettings;
extern uint8_t HH, MM, SS;
extern uint8_t inputRepeat;

void go2Field(uint8_t Pos) {
    if(inputRepeat){
	field *Field = AppSettings.currentLayer->Fields[0];	
	uint8_t PosX = Pos & 0x3F;
	uint8_t PosY = (Pos & 0xC0) >> 6;
	for (uint8_t count = 0; count<NUMBEROFUSEDINPUTS; count++){
	    Field = AppSettings.currentLayer->Fields[count];
	    if(Field->Pos.PosX == PosX && Field->Pos.PosY == PosY){
		AppSettings.Status = 4;
		AppSettings.currentField = Field;
	    }
	}
    }
}

void go2Layer(uint8_t LayerId){
	uint8_t Index = 0;
	layer *Layers = AppSettings.Layers[Index];	
	while(Layers != NULL && Layers->Id != LayerId){
		Index++;
		Layers = AppSettings.Layers[Index];
	}

	if(Layers != NULL){
		LCD_CLR();
		AppSettings.currentLayer = Layers;
		AppSettings.Status = 7;
		// first field is entry point
		AppSettings.currentField = Layers->Fields[0];
	}
}

// Change The Number on the Display.
void changeNumber(uint8_t changeValue){
    switch(changeValue){
	case 1:
	    changeTimeNumber(&HH, 24);
	    break;
	case 2:
	    changeTimeNumber(&MM, 60);
	    break;
	case 3:
	    changeTimeNumber(&SS, 60);
	    break;
    }
}


// function will change the time number on the Display.
void changeTimeNumber(uint8_t *Time, uint8_t maxVal){
    CUR_OFF();
    CURB_OFF();
    ((*Time)<(maxVal-1))?((*Time)++):((*Time)=0);
    setPosition(getPosition()+1);
    LCD_OutTwoNumbers(*Time);
    setPosition(getPosition()-3);
    CUR_ON();
    CURB_ON();
    
}
