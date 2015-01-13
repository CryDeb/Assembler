/******************************************************************************
;* Programm:	Protocol implementation		
;*
;* Dateinname: Protocol.c
;* Version:		 0.1
;* Autor:			 Kasipillai	Sukirtha
;*
;* Verwendungszweck:	uP-Schulung
;*
;* Beschreibung:
;*	Diese Projekt ist eine Implementation eines einfachen Bussystem-Protokolls.
;*  Dabei sollen verschiedene Microcontroller am Bussystem hängen und unterein-
;*	ander kommuniziern können.
;*  
;*  HINWEIS:
;*  Das Projekt ist noch nicht fertig. Bisher ist nur ein Teil der Menu Steuerung
;*  und der Physical Layer komplett.
;*  
;*  Es kann durch den Physical Layer über einen Pin kommuniziert werden 
;*	(Empfange und Versenden) siehe PhyLayer.c für Beschreibung.
;*		
;*				
;*
;* Entwicklungsablauf:
;* Ver: Datum:	Autor:    Entwicklungsschritt:
;* 1.0  01.01.12  DWi/SKa siehe verlauf https://github.com/CryDeb/Assembler/
;*
;*										Totalzeit:	19.01.2014 - 17.05.2014
;******************************************************************************/

//includes
#include "protocol.h"
//defines
#define MODE RX


/*******************************************************************************
* Funktion:  main
*									   									   
* Autor:     WD / KS						  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05. 2014						   									   
********************************************************************************
* Beschreibung:
*	Hauptrouting des Programms. Initialsiert das Menu und die Kommunikation. Da-
*	nach wird das Menu betrieben und die eikommenden Command abgearbeitet.
*
*
*******************************************************************************/
int main() {
	DDRC = 0xFF;
	DDRB = 0x00;
	uint8_t b[] = {2,6,7};
	frame_t frame = {1,2,3,4,0,&b};
	int Byte = 0x12; 

	// init communication & LCD interfaces
	initMenu();
	initPhyLayer();
	initLayer();
	initJobs();
	setMode(MODE);
	W1s();W1s();
	// compiler for AVR Studio 4.19 doesn't support 
	// extended preprocessor directives
	if(MODE == RX)
		startListening(receiveFrame);
	else
		W1s();W1s();W1s();
	// start LCD GUI
	while(TRUE){
		/*process(scanCode());
		display();
		*/
		if(MODE == TX) {
			//putByteAsync(Byte,1);
			if(PINB != 0xFF)
				sendFrame(&frame);
			Byte = ~Byte;

		} else {
			handleIncomingTask();
		}
		W1s();W1s();//W1s();W1s();W1s();
	}
}
