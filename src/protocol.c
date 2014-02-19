#include <avr/io.h>
#include <stdint.h>
#define F_CPU 3686400UL     /* Quarz mit 3.6864 Mhz */
#include <util/delay.h>   
/* Defines für Includes */
/* LCD Port */
#define LCD 	PORTC
#define LCD_D 	DDRC
/* Input port */
#define KEYS 	DDRA
#define ROW		PORTA
#define COLUMN	PINA
/* Phy Port */
#define PHYPort		PORTB
#define PHYPort_D	DDRB
#define PHYPin		PB1
/* Lib includes */
#include "input.h"
#include "display.h"
#include "menu.h"
/* Defines */
#define TRUE 1
#define FALSE 0

int main(){
	initMenu();

	while(TRUE){
		process(scanCode());
		display();
	}
}


