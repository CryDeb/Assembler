#include <avr/io.h>
#include <stdint.h>
#define F_CPU 3686400L     /* Quarz mit 3.6864 Mhz */
#include <util/delay.h>   
/* Defines für Includes */
/* LCD Port */
#define LCD 	PORTA
#define LCD_D 	DDRA
/* Input port */
#define KEYS 	DDRA
#define ROW		PORTA
#define COLUMN	PINA
/* Phy Port */
#define PhyPortTX	PORTD
#define PhyPortRX 	PIND
#define PhyPort_D	DDRD
#define PhyPin		PD3  
/* Lib includes */
#include "input.h"
#include "display.h"
#include "menu.h"
#include "PhyLayer.h"
#include "FrameLayer.h"
/* Defines */
#define TRUE 1
#define FALSE 0

extern uint8_t Byte;

int main(){
	DDRC = 0xFF;
	initMenu();
	initPhyLayer();
//	setMode(TX);
	frame_t Frame = {
		1,1,0,0,1,0
	};
	W1s();
	while(TRUE){
		LCD_STR("Waiting");
		W1s();
		LCD_CLR();
		/*process(scanCode());
		display();*/
		/*
		putByte(0x0F);
		W1s();W1s();
		putByte(0xF0);
		W1s();W1s();
		/*/
		
				
	}
}


