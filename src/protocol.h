#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/* std includes */
#include <avr/io.h>
#include <stdint.h>
#ifndef F_CPU
#define F_CPU 3686400L     /* Quarz mit 3.6864 Mhz */
#endif
#include <util/delay.h>   

/* Defines for includes */
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


#endif
