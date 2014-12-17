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
// Enthaelt alle Layer, neue Layer müssen hier angegeben werden.
settings AppSettings = {
	.Layers = {
		&SettingsLayer,
		&ShowTimeLayer,
		&ChangeTimeLayer
	}
};
//Stunden, Minuten, Sekunden
uint8_t HH = 02, MM = 50 ,SS = 15;
//Eingabe Wiederunholungsflag
uint8_t inputRepeat=1;
/*******************************************************************************
* Funktion:  initMenu
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: initMenu()
*
* Beschreibung:
*	Initialisiert das LCD und ladet die erste Ebene
*
* Precondition: 
*	Die Defines für das LCD müssen bzw. sollten angegeben sein, falls nicht
*	der default PORTA genommen werden soll.
*
*******************************************************************************/
void initMenu() {
	_delay_ms(20);
	LCD_Init();

	print_splash();
	AppSettings.currentLayer = &ShowTimeLayer;
	AppSettings.currentField = AppSettings.currentLayer->Fields[0];
	AppSettings.Status = 7;
	//display();
}
/*******************************************************************************
* Funktion:  print_splash
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: print_splash
*
* Beschreibung:
*	Hilfsmethode um anfangsausgabe auszuführen. Stellt ein Bootscreen dar.
*
*******************************************************************************/
void print_splash() {
	LCD_STR("Protocol Example");
	setPosition_XY(0, 2);
	LCD_STR("c DW/SK");
	_delay_ms(1000);
	LCD_CLR();
}
/*******************************************************************************
* Funktion:  display
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: display
*
* Beschreibung:
*	Ruft die update methode des aktuellen layers auf. Hier wird das GUI aktual-
*	lisiert.
*
* Precondition: 
*	Ein gültier aktulle Layer muss im AppSettings eingestellt sein.
*
*******************************************************************************/
void display() {
	AppSettings.currentLayer->update(AppSettings.Status);
	// clear flags after update
	AppSettings.Status = 0;
}
/*******************************************************************************
* Funktion:  process
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: process(${EINGABE});
*
* Beschreibung:
*	Verwertet den Eingegebenen Tastendruck. Falls keine Taste gedrückt wurde (FF)
*	wird nichts gemacht.
*	Anhand des Wertes wird ermittel welcher Kommand ausgeführt werden soll.
*	Es UP,LEFT,ENTER,RIGHT und DOWN unterstützt für den MAP auf die Tasten siehe
*	input.c
*	
* Daten:
*	uint8_t Input	Wert der Eingelesen wurde
*
*******************************************************************************/
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
