#include <stdint.h>
#include <stddef.h>
#include "menu.h"
#include "commands.h"
#include "display.h"

extern settings AppSettings;

void go2Field(uint8_t Pos) {
	field *Field = AppSettings.currentLayer->Fields[0];
	uint8_t PosX = (Pos & 0xC0) >> 6;
	uint8_t PosY = Pos & 0x3F;
	uint8_t Index = 0;

	while(Field != NULL && Field->Pos.PosX != PosX && Field->Pos.PosY != PosY ) {
		Field = AppSettings.currentLayer->Fields[Index++];
	}
	
	if(Field != NULL){
		AppSettings.currentField = Field;
	};
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
		// erstes feld ist einstiegspunkt
		AppSettings.currentField = Layers->Fields[0];
	}
}



