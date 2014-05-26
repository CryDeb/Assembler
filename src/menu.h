#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>


typedef struct position {
	uint8_t PosX:6;
	uint8_t PosY:2;
} position;

typedef void (*com)(uint8_t param);

typedef struct command {
	com Payload;
	uint8_t Arg;
} command;

typedef struct field {
	position Pos;
	command Up; 
	command Left;
	command Enter;
	command Right;
	command Down;
} field;

typedef struct layer {
	uint8_t Id;
	com 	update;
	field 	*Fields[];
} layer;

typedef struct settings {
	uint8_t	  mpr;
	uint8_t	  Status:3; // 0b00000CSD (Cursor,Static,Dynamic)
	uint8_t	  Modus:1; // VIEW/EDIT
	layer	  *currentLayer;
	field	  *currentField;
	layer	  *Layers[];
} settings;

typedef enum inputState {
	UP = 5, LEFT = 2, ENTER=6, RIGHT=10, DOWN = 7
} inputState;

#define NUMBEROFUSEDINPUTS 5

void initMenu();
void print_splash();
void display();
void process(uint8_t Input);

#endif // #ifndef __MENU_H__
