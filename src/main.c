#include <avr/io.h>
#include <stdint.h>
#ifndef F_CPU
#define F_CPU 3686400L     /* Quarz mit 3.6864 Mhz */
#endif
#include <util/delay.h>   
/* Defines f�r Includes */
/* LCD Port */
#define LCD		PORTA
#define LCD_D		DDRA
/* Input port */
#define KEYS		DDRA
#define ROW		PORTA
#define COLUMN		PINA
/* Phy Port */
#define PhyPortTX	PORTD
#define PhyPortRX	PIND
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

//extern uint8_t Byte;
#define MODE RX

#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "display.h"
#include "delay.h"
#include "menu.h"
#include "PhyLayer.h"
#include "FrameLayer.h"

/* Defines */
#define TRUE 1
#define FALSE 0

//extern uint8_t Byte;
#define MODE RX
/*
 * 
 */
int main() {
    //LCD_Init();
    //LCD_STR("Protocol Example");
    startListening(receiveFrame);
    initMenu();
    CURB_ON();
    while(1==1){
	process(scanCode());
	//LCD_INT((3<<6)|(6)); //11000110
	display();
    }
    return (1);
}

