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

//extern uint8_t Byte;
#define MODE RX

int main(){
	DDRC = 0xFF;
	uint8_t Tmp = 0;
	/*
	initMenu();
	//*/
	initPhyLayer();
	setMode(MODE);
	/*
	 Destination;
	 Source;
	 SequenceNumL;
	 SequenceNumH;
	 CommandNumber;
	 Parameters;*/

  uint8_t b[] = {3,6,7,8};
	frame_t Frame = {1,2,3,4,5, .Parameters = b};

	startListening(receiveFrame);
	while(TRUE){
		if(MODE == TX)
		{
			/*putByteAsync(0x55,TRUE);
			W1s();W1s();
			putByteAsync(0xAA, TRUE);
			W1s();W1s();*/
//			putByteAsync(i,TRUE);
			sendFrame(&Frame);

		W100ms();
		}
		else
		{
			//getByteAsync(TRUE);
			W10ms();
			//W1s();W1s();
			//getByteAsync();
		}
	}
}


