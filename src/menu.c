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
		&SettingsLayer,
		&ShowTimeLayer,
		NULL
	}
};

uint8_t HH,MM,SS;

void initMenu() {
	_delay_ms(20);
	LCD_Init();

	print_splash();
	/*AppSettings.currentField = &TimeField;
	AppSettings.currentLayer = &SettingsLayer;
	AppSettings.Status = 7;*/
}

void print_splash() {
	LCD_STR("Protocol Example");
	LCD_RAM(80);
	LCD_STR("c Wicki/Kasipillai");
	_delay_ms(3000);
	LCD_CLR();
}

void display() {
	AppSettings.currentLayer->update(AppSettings.Status);
	// nach update flags wieder loeschen
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
		}		
		if(Command.Payload != NULL) {
			Command.Payload(Command.Arg);
		}
	}
}

