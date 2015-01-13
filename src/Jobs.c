#include <stdint.h>
#include <avr/io.h>
#include "Jobs.h"
#include <util/delay.h>  

void printToPortC(frame_t *Frame){
	for(int i = 1; i < Frame->Parameters[0]; i++) {
		PORTC = Frame->Parameters[i];
		W1s();
	}
}
