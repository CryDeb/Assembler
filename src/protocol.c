#include "protocol.h"

#define MODE TX

int main() {
	DDRC = 0xFF;
	// init communication & LCD interfaces
	initMenu();
	initPhyLayer();
	setMode(MODE);
	//TODO
	// compiler for AVR Studio 4.19 doesn't support 
	// extended preprocessor directives
	if(MODE == RX)
		startListening(receiveFrame);
	// start LCD GUI
	while(TRUE){
		process(scanCode());
		display();
		handleIncomingTask();
	}
}
