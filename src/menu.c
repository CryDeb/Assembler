#include <stddef.h>
#define F_CPU 3686400UL     /* Quarz mit 3.6864 Mhz */
#include <util/delay.h>
#include "input.h" 
#include "menu.h"
#include "display.h"
#include "layers.h"
#include "layercontent.h"
#include "fields.h"
#include "commands.h"

settings AppSettings = {
	.Layers = {
		&editTextLayer
	}
};
char SendingText[46]={32};
char getingText[46];
uint8_t inputRepeat=1;

void initMenu() {
	_delay_ms(20);
	LCD_Init();
	print_splash();
	AppSettings.currentLayer = &editTextLayer;
	AppSettings.currentField = AppSettings.currentLayer->Fields[0];
	AppSettings.Status = 7;
	display();
}

void print_splash() {
	LCD_STR("Protocol Example");
	setPosition_XY(0, 2);
	LCD_STR("c DW/SK");
	_delay_ms(1000);
	LCD_CLR();
}

void display() {
	AppSettings.currentLayer->update(AppSettings.Status);
	// clear flags after update
	AppSettings.Status = 0;
}

void process(uint8_t Input) {
	if(Input != 0xFF) {
		field *Field = AppSettings.currentField;
		command Command = {NULL};
		switch(Input) {
		case UP:
			Command = Field->Up;
			break;
		case LEFT:
			Command = Field->Left;
			break;
		case ENTER:
			Command = Field->Enter;
			break;
		case RIGHT:
			Command = Field->Right;
			break;
		case DOWN:
			Command = Field->Down;
			break;
		case ABCDOWN:
			Command = Field->AbcDown;
			break;
		case ABCUP:
			Command = Field->AbcUp;
			break;
		}		
		if(Command.Payload != NULL) {
			Command.Payload(Command.Arg);
		}
	}
}
