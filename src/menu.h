#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>

//stellt eine position auf dem LCD dar
typedef struct position {
	uint8_t PosX:6;
	uint8_t PosY:2;
} position;

//diese interface müssen alle Layer update
//und alle Tasten hander befolgen
typedef void (*com)(uint8_t param);

//definiertes kommand besitzt ein funk-pointer und
//ein argument für handler
typedef struct command {
	com Payload;
	uint8_t Arg;
} command;

// stellt ein feld dar
// hat handler für alle möglichen tasten
typedef struct field {
	// positon  auf LCD
	position Pos;
	// handler für entsprechnden tastendruck
	command Up; 
	command Left;
	command Enter;
	command Right;
	command Down;
} field;

//layer beinhaltet alle felder und eine handler um gui zu zeichnen
typedef struct layer {
	// layer id muss eindeutig sein 
	uint8_t Id;
	// handler wird aufgerugen um gui zu zeichnenn
	com 	update;
	// alle zugewiesene felder
	field 	*Fields[];
} layer;

// beinhaltet alle Ebenen der Applikation und weiter steuer variablen
typedef struct settings {
	// temp variable keine feste anwendung
	uint8_t	  mpr;
	// 0b00000CSD (Cursor,Static,Dynamic)
	// flag welcher GUI Bereich erneuert werden muss
	uint8_t	  Status:3;
	// VIEW/EDIT 
	uint8_t	  Modus:1; 
	// aktuel eingestellter Layer
	layer	  *currentLayer;
	// aktuelles feld
	field	  *currentField;
	// alle layer
	layer	  *Layers[];
} settings;

//mögliche eingaben und die entsprecheden Tasten Werte
typedef enum inputState {
	UP = 5, LEFT = 2, ENTER=6, RIGHT=10, DOWN = 7
} inputState;

#define NUMBEROFUSEDINPUTS 5

/* prototypes */
void initMenu();
void print_splash();
void display();
void process(uint8_t Input);

#endif // #ifndef __MENU_H__
