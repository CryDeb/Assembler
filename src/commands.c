#include <stdint.h>
#include <stddef.h>
#include "menu.h"
#include "commands.h"
#include "display.h"
#include "FrameLayer.h"
#include <stdio.h>
#include <stdlib.h>

extern field editText;
extern settings AppSettings;
extern char SendingText[];
extern uint8_t inputRepeat;

void go2nextField(uint8_t pDirection){
	field *Field = AppSettings.currentLayer->Fields[0];
	uint8_t X = AppSettings.currentField->Pos.PosX;
	uint8_t Y = AppSettings.currentField->Pos.PosY;
	uint8_t FieldArrayLength = 
		sizeof(*AppSettings.currentLayer->Fields) /
		sizeof(AppSettings.currentLayer->Fields[0]);
	switch(pDirection){
	case _RIGHT_:
		for(uint8_t count=0; count<FieldArrayLength;count++){
			Field = AppSettings.currentLayer->Fields[count];
			if(Field->Pos.PosX > X)
				saveField(Field);
		}
		break;
	case _LEFT_:
		for(uint8_t count=0; count<FieldArrayLength;count++){
			Field = AppSettings.currentLayer->Fields[count];
			if(Field->Pos.PosX < X)
				saveField(Field);
		}
		break;
	case _DOWN_:
		for(uint8_t count=0; count<FieldArrayLength;count++){
			Field = AppSettings.currentLayer->Fields[count];
			if(Field->Pos.PosY > Y)
				saveField(Field);
		}
		break;
	case _UP_:
		for(uint8_t count=0; count<FieldArrayLength;count++){
			Field = AppSettings.currentLayer->Fields[count];
			if(Field->Pos.PosY < Y)
				saveField(Field);
		}
		break;
	}
}

void saveField(field *pField){
	AppSettings.Status = 4;
	AppSettings.currentField = pField;
}

void go2nextFieldfree(uint8_t pDirection){
	uint8_t X = AppSettings.currentField->Pos.PosX;
	uint8_t Y = AppSettings.currentField->Pos.PosY;
	switch(pDirection){
	case _RIGHT_:
		if(X!=14)
			X++;
		else
			go2nextField(_RIGHT_);
		break;
	case _LEFT_:
		if(X!=0)
			X--;
		break;
	case _DOWN_:
		if(X!=2)
			Y++;
		else
			go2nextField(_DOWN_);
		break;
	case _UP_:
		if(Y!=0)
			Y--;
		break;
	}
	go2Field((Y<<6)|X);
}

void go2Field(uint8_t Pos) {
	if(inputRepeat){
		editText.Pos.PosX = Pos & 0x3F;
		editText.Pos.PosY = (Pos & 0xC0) >> 6;
		AppSettings.Status = 4;
		AppSettings.currentField = &editText;
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

void changeText(uint8_t pDirection){
	uint8_t numbArray = AppSettings.currentField->Pos.PosX + (AppSettings.currentField->Pos.PosY * 15);
	switch (pDirection){
	case _UP_:
		SendingText[numbArray]++;
		LCD_CHR(SendingText[numbArray]);
		setPosition(getPosition()-1);
		break;
	case _DOWN_:
		SendingText[numbArray]--;
		LCD_CHR(SendingText[numbArray]);
		setPosition(getPosition()-1);
		break;
	}
	
}
void sendTextToNext(uint8_t null){
	frame_t Frame = {1,2,3,4,5, .Parameters = SendingText};
	sendFrame(&Frame);
}