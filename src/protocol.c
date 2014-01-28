#include <avr/io.h>
#include <stdint.h>
#define F_CPU 3686400UL     /* Quarz mit 3.6864 Mhz */
#include <util/delay.h>   
#include "input.h"
#include "display.h"
#include "menu.h"


#define TRUE 1
#define FALSE 0

#define LED		PORTB
#define LED_D 	DDRB

/**
	1
	|
  4-5-6
	|
	9
*/

#define wait _delay_ms(1000);_delay_ms(1000);_delay_ms(1000);_delay_ms(1000)

int main(){
	init();

	while(TRUE){
		process(scanCode());
		display();
	}
}


