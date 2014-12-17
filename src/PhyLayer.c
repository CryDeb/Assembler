#include "PhyLayer.h"
#include "ringbuffer.h"
#include <avr/interrupt.h>
/*******************************************************************************
* Hinweis: Noch in bearbeitung
*******************************************************************************/
//handler falls ein byte eingelesen wurde
volatile ByteReceivedHandler receivedByte = NULL;
//Modus des PhyLayer RX oder TX
volatile PHYModes CommunicationMode;
//aktueller bit status beim lesen oder schreiben
volatile ComunicationState CurrentState;
//letzter error
volatile CommunicationError LastError = SUCCESS;
//temp für momentaner byte wert
volatile uint8_t Frame;
//counter für anzahl ausgegebene bits
uint8_t NumBitCounter;
//ringbuffer verweiss
RingBuffer *Buffer = NULL;
// flag ob Listening Mode beendet werden muss
volatile uint8_t stopListeningMode = 0;
//Zeit die abgewartet wird nach einer Kollision
uint8_t JamWaitTime;
//lokale adresse
uint8_t LocalAddress;
/* Defines */
//#define USE_TIMER0
// wert des pins
#define PIN (1 << (PhyPin - 1))
#define NOP asm volatile ("nop")
#define SET_LOW(x) ((x) &= ~PIN)
#define SET_HIGH(x) ((x) |= PIN)
#define GET_STATE(x) ((x) & PIN)
#define MIN_WAIT_CYCLE 20
/*******************************************************************************
* Funktion:  initPhyLayer
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: initPhyLayer()
*
* Beschreibung:
*	Initialisiert den Physical Layer & stellt alle Einstellung und Ressourcen
*	bereit. Für die korrekte verarbeitung werden folgende Interrupts reserviert. 
*	Reservierte mC Ports/Pin:
*		INT0 erkennung des Startbits
*		INT1 wird ausgelöst falls eine entsprechender Handler für receivedByte
*			 registriert ist (receivedByte != NULL)
*		TIMER0 wird für die bit ausgabe bzw. einlesen der bits benötigt
*
* Postcondition:
*	Erstellt einen Ringbuffer, dieser alloziert speicher muss wieder frei ge-
*	geben werden.	
*
*******************************************************************************/
CommunicationError initPhyLayer() {
	CurrentState = CS_FINISHED;

#ifdef USE_TIMER0
	OCR0 = 255;
	TCCR0 = 0x00;
	TCCR0 = (1 << CS00);
#else
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A = (1 << WGM12) | (1 << COM1A0);

	TCCR1B = /*(1 << CS10) |*/(1 << CS11);
	OCR1A = CYCLES;
#endif

	Buffer = createRingBuffer(BUFFERSIZE);
	return SUCCESS;
}
/*******************************************************************************
* Funktion:  setMode
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: setMode(${MODUS});
*
* Beschreibung:
*	setzt den Modus des Physical Layers, muss gesetzt sein bevor kommuniziert
*	werden kann. Zusätzlich wird hier der Kommunikationsport anhand des modus
*	eingestellt. Zudem werden die Interrupteinstellung für den entsprechenden
*	Modus vorgenommen:
*	Mods:
*	 RX:
*		Prescale für Timer
*		PIN3 als Input und PIN4 als Output für PORTD
*		INT0 und INT1 auf fallende flank triggern
*	 TX:
*		INT0 deaktiviern (INT1 kann ignoriert werden)
*		PIN3 als output setzten (PORTD)	
*
* Precondition:
*	Der Modus kann nur gewächselt werden wenn keine Kommunikation statt findet.
*	Ansonsten wird INVALIDSTATE zurückgegeben	
*
* Daten:
*	PHYModes Modes	neuer Modus
*
*******************************************************************************/
CommunicationError setMode(PHYModes Mode) {
	CommunicationError Error = SUCCESS;
	if(CurrentState == CS_FINISHED)
	{
		
		CommunicationMode = Mode;
		if(Mode == RX) {
			// set to input
			PhyPort_D &= ~(PIN);
			// set pull up
			PhyPortTX |= PIN;
			// Trigger INT on falling edge
			MCUCR = INTMode;
			// set PIN4 as output and trigger INT1 on falling edge
			PhyPort_D |= 8;
			MCUCR |= 1 << ISC11;
					
		} else {
			// set to output
			PhyPort_D |= PIN;
			// disable INT0
			GICR &= ~(1<<INTPin);
		}
	} else {
		Error = INVALIDSTATE;
	}
	return LastError = Error;
}
/*******************************************************************************
* Funktion:  getLastError
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: getLastError
*
* Beschreibung:
*	Gibt letzten Error zurück
*******************************************************************************/
CommunicationError getLastError() {
	return LastError;
}
/*******************************************************************************
* Funktion:  putByte
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: putByte(${BYTE});
*
* Beschreibung:
*	schreibt ein Byte raus, falls mommentan kommuniziert wird block dieser aufruf
*	
*
* Daten:
*	uint8_t Byte	Wert der geschrieben werden soll
*
*******************************************************************************/
CommunicationError putByte(uint8_t Byte) {
	CommunicationError Error = SUCCESS;
	
	putByteAsync(Byte, TRUE);
	while(CurrentState != CS_FINISHED) NOP;
	W100us();

	return LastError = Error;
	
}
/*******************************************************************************
* Funktion:  getByte
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: getByte(${EINGELESENER BYTE});
*
* Beschreibung:
*	Liesst einen Byte ein, falls keine im RingBufer vorhanden sind, wird solange
*	gewartet bis einer gelesen wurde.
*
* Precondition: 
*	RingBuffer muss initialisiert sein.
*	
*
* Daten:
*	uint8_t* out Byte 	Eingelesener Byte
*
*******************************************************************************/
CommunicationError getByte(uint8_t *Byte) {
	CommunicationError Error = SUCCESS;
	int16_t Data = getEntry(Buffer);
	
	if(Data == BUFFEREMPTY) {
		// start async recv and wait
		Error = getByteAsync(TRUE);
		while(CurrentState != CS_FINISHED) NOP;
		Data = getEntry(Buffer);
	}		

	if(Data > 0) {
		Byte = (uint8_t)Data;
	}

	return LastError = Error;
}
/*******************************************************************************
* Funktion:  putByteAsync
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: putByteAsync(${WERT},${FALG OB BLOCKIEREND});
*
* Beschreibung:
*	schreibt eine Byte aus, falls kommuniziert wird und ${Bock} nicht true ist
*	wird eine WOULDBLOCK Error zurück gegeben. ansonsten wird gewartet bis das
*	Byte versendet werden kann. Der versand geschieht asynchron. Das Byte wird
*	dem Frame zugewiesen und der Timer wird gestartet.
*
*
* Postcondition:
*	Kommunikation und Timer laufen	
*
* Daten:
*	uint8_t Byte	zu schreibender Wert
*	uint8_t Block	flag ob geblockt werden soll falls kommunziert wird
*
*******************************************************************************/
CommunicationError putByteAsync(uint8_t Byte, uint8_t Block) {
	CommunicationError Error = SUCCESS;
	
	if(CurrentState != CS_FINISHED) {
		
		if(Block) {
			
			while(CurrentState != CS_FINISHED) NOP;
		 } else {
			Error == WOULDBLOCK;
		}
	}
	if(Error == SUCCESS) {
		setMode(TX);
		Frame = Byte;
		CurrentState = CS_STARTBIT;
		
		//enable timer
#ifdef USE_TIMER0	
		TCCR0 = (1 << CS00);
		TIMSK = (1 << TOIE0);
#else
		TCCR1B = /*(1 << CS10) |*/(1 << CS11);
		TIMSK |= (1 << OCIE1A);
#endif
		sei();
	}

	return LastError = Error;
}
/*******************************************************************************
* Funktion:  getByteAsync
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: getByteAsync(${FALG OB BLOCKIEREND});
*
* Beschreibung:
*	Asynchron ein Byte lesen, das gelesene Byte wird in den RingBuffer 
*	geschrieben. Falls ${Block} true ist und schon kommuniziert wird, wird
*	geblockt. Das lesen geschieht asynchron, INT0 wird enabled.
*
* Daten:
*	uint8_t Block	flag ob geblockt werden soll falls kommunziert wird
*
*******************************************************************************/
CommunicationError getByteAsync(uint8_t Block) {
	CommunicationError Error = SUCCESS;
	
	if(CurrentState != CS_FINISHED && Block) {
		Error == WOULDBLOCK;
	} else {
		while(CurrentState != CS_FINISHED) NOP;
		setMode(RX);
		if(CommunicationMode == RX)
		{
			// clear INT0
			GIFR |= (1<<INTF0);
			// Enable INT0
			GICR |= 1<<INT0;
			sei();
		}
	}
	
	return LastError = Error;
}

/*******************************************************************************
* Funktion:  getJamWaitTime
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 15.10.2014  							   									   
********************************************************************************
* Aufruf: getJamWaitTime();
*
* Beschreibung:
*	Berechnet die Zeit die abgwartet wird, die bei einer Kollision abgewartet 
*	werden muss. Es wird das 10-fache der Adress plus eine minimale Anzahl 
*	Cycles gerechnet
*	
*
* Daten:
*	uint8_t RETURN	Wartezeit
*
*******************************************************************************/
uint8_t getJamWaitTime()
{
	return MIN_WAIT_CYCLE + 10 * LocalAddress;
}

/*******************************************************************************
* Funktion:  startListening
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: startListening(${HANDLER});
*
* Beschreibung:
*	Aktiviert die Interrupts (INT0 und INT1) so das eingelesen werden kann
*	und registriert einen Handler der aufgerufen wird wenn ein Byte eingelesen
*	wurde
*
* Precondition: 
*	Der Handler muss != NULL sein
*
* Postcondition:
*	Es werden auf einkommende Bytes geschaut
*	
*
* Daten:
*	ByteReceivedHandler Handler 	Funktions die aufgerufen werden soll falls
*									ein Byte gelesen wurde
*
*******************************************************************************/
CommunicationError startListening(ByteReceivedHandler Handler) {
	if(Handler != NULL) {
		receivedByte = Handler;
		// clear INT1
		GIFR |= (1<<INTF1);
		// Enable INT1
		GICR |= 1<<INT1;
		setMode(RX);
		// clear INT0
		GIFR |= (1<<INTF0);
		// Enable INT0
		GICR |= 1<<INT0;
		sei();
	}
}
/*******************************************************************************
* Funktion:  stopListening
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: stopListening()
*
* Beschreibung:
*	Stoppt das Listen auf den Kommunikationskanal. Der receive Handler wird
*	entfernt und INT0 wird deaktiviert, falls nicht kommuniziert wird. Ansonsten
*	wird der Flag gesetzt und der Listening Mode wird beendet nach dem das 
*	aktuell eingelesene Byte fertig ist.
*
* Postcondition:
*	Nach dieser Routine werden keine weiteren Bytes eingelsen. 
*	
*
* Daten:
*
*******************************************************************************/
CommunicationError stopListening() {
	if(CurrentState != CS_FINISHED) {
		stopListeningMode = 1;
	} else {
		GICR &= ~(1<<INT1);	//disable INT0
		receivedByte = NULL;
	}
	return SUCCESS;
}

/**** Interrupt Routinen *****/

/*******************************************************************************
* Funktion:  ISR(INT1_vect) 
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Beschreibung:
*	Interrupt Routing wird ausgelöst wenn eine Byte fertig geschrieben wurde und
*	ein entsprechnder Handler registriert ist.
*
* Precondition: 
*	receivedByte Handler muss registriert sein.
*	
*
* Postcondition:
*	Eingelesenes Byte wird aus dem Ringbuffer entfernt	
*
* Daten:
*
*******************************************************************************/
ISR(INT1_vect) {
	//execute function pointer
	int16_t	Data = getEntry(Buffer);

	if(Data >= 0) {		
		receivedByte((uint8_t)Data);
	}
}

/*******************************************************************************
* Funktion:  ISR(INT0_vect)
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Beschreibung:
*	Interruptroutine erkennt ein Startbit beim receive, fall aktive.
*	Falls eine Interrupt ausgelöst wird, wird der Timer mit der hälfte (siehe 
*	Timer) der Bitsignal Länge gestartet und INT0 wird deaktiviert (sodass
*	während kommunikation nicht getriggert wird)
*
* Precondition: 
*	Keine Kommunikation sollte laufen
*
* Postcondition:
*	Timer wird gestartet, dieser Interrup wird disabled
*	
*******************************************************************************/
ISR(INT0_vect) {
	CurrentState = CS_STARTBIT;
#ifdef USE_TIMER0
	OCR0 = 128;
	TCCR0 = (1 << CS00);
	TIMSK = (1 << TOIE0);
#else
	OCR1A = CYCLES;	
	TCCR1B = /*(1 << CS10) |*/(1 << CS11);
	TIMSK |= (1 << OCIE1A);
#endif
	GICR &= ~(1<<INTPin);	//disable INT0 
}

/*******************************************************************************
* Funktion:  ISR(TIMER0_COMP_vect)
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* HINWEIS: Kollisions abhandlung noch nicht implementiert
*
* Beschreibung:
*	Interrupt Routiner für Compare Timer. Diese gibt das Byte bitweise auf das
*	Port aus bzw. liset ein Bit ein. Dabei wird jedes mal nur ein Bit behandelt.
*	Anhand des Kommunikations Modus wird entschieden ob gelesen oder geschrieben
*	werden muss.
*	Zusätzlich zum Daten Byte werden noch zwei weiter Bits geschickt 
*	Aufbau eines Frames:
*	---\_XXXXXXX	X/---	
*	  | Daten     |
*	Zuerst wird der Zustands des Pin auf 0 gesetzt (\) danach können die 
*	Bits geschickt werden (X) zum abschluss muss der Zustand wieder auf
*	1 gesetzt werden (/)
*
*	TX: 
*		Wert wird bitweise geschrieben nachdem alle Bits raus sind wird der 
*		Timer beendet.
*	RX:
*		Hier werden die Bits ebenfalls Bitweise eingelesen. Speziel ist
*		hier um die hälfte der Periodendauer eies Bits gelesen wird. Dies
*		stellt sicher dass der vom Host geschriebene Wert stabil ist.
*		Nachdem ein Byte erfolgreich gelesen wurde, wird es in den RingBuffer
*		hinzugefügt.
*		
*
* Precondition: 
*	Beim anfang eines Receives muss die hälfte der Periodendauer abgewartet 
*	werden, dies ist so implementiert:
*	Nachdem ein Startbit erkannt wurde (INT0) wird der Timer mit der hälfte
*	des eigentlichen Counterwertes gestartet. So ist man doppelt so schnell
*	in der dieser Interruptroutine. Dies bedeutet man ist ungefähr in der 
*	hälfte der Bit signals. Nun kann der Counterwert wieder verdoppelt werden,
*	ab hier wird wieder eine ganze Periode gewartet. Durch die Verschiebung
*	wird der Timer immer ungefähr in der Mitte des Bitsignals ausgelöst.
*
* Postcondition:
*	Nachdem ein Bit eingelesen wurde, wird die entsprechende State Machine 
*	aktualisiert. Zudem wird der Bit in Frame hinzugefügt.
*	Falls Byte fertig eingelesen wurde, wird das Byte dem RingBuffer übergeben
*
*******************************************************************************/
#ifdef USE_TIMER0
 ISR(TIMER0_OVF_vect)
#else
 ISR(TIMER1_COMPA_vect)
#endif
{
	uint8_t Sent = 0;
	switch(CommunicationMode) {
		case TX:
			// check ob letztes gesendetes bit noch auf linie 
			/*if(CurrentState != CS_FINISHED && CurrentState != CS_SENDJAM && GET_STATE(PhyPortTX) != GET_STATE(Sent)) {
				CurrentState = CS_SENDJAM;
				JamWaitTime = getJamWaitTime();
				NumBitCounter = 0;
			}*/
			switch(CurrentState) {
				case CS_STARTBIT:
					NumBitCounter = 0;
					// start indizieren
					Sent = SET_LOW(PhyPortTX);
					CurrentState = CS_FRAMEBIT;
					break;
				case CS_FRAMEBIT:
					
					// bits senden
					Sent = ((Frame >> NumBitCounter) & 0x01) ? SET_HIGH(PhyPortTX) : SET_LOW(PhyPortTX);

					if(++NumBitCounter >=  8)
						CurrentState = CS_STOPBIT;
					break;
				case CS_SENDJAM:
					SET_LOW(PhyPortTX);
	
					if(++NumBitCounter >= JamWaitTime)
						CurrentState = CS_STOPBIT;
					break;
				case CS_STOPBIT:				
					// stop bit senden
					Sent = SET_HIGH(PhyPortTX);
					

					PORTC = Frame;
					// stop timer
#ifdef USE_TIMER0
					TCCR0 = 0;
					TIMSK &= ~(1 << TOIE0);
#else 	
					TCCR1B = 0;
					TIMSK &= ~(1 << OCIE1A);
#endif
					CurrentState = CS_FINISHED;
					break;			
			}
			
			break;
		case RX:
			switch(CurrentState) {
				case CS_STARTBIT:
					Frame = 0;
					
					NumBitCounter = 0;
					CurrentState = CS_FRAMEBIT;
					// anzahl zyklen wieder setzen
					OCR1A = CYCLES;
					OCR0 = 255;
					break;
				case CS_FRAMEBIT:
					// receive lsb to msb
					Frame |= (((PhyPortRX) & PIN)) ? 0 : 1 << NumBitCounter;
					
					
					if(++NumBitCounter >= 8)
					{
						CurrentState = CS_STOPBIT;
						
					}
					break;
				case CS_STOPBIT:	
					GIFR |= (1<<INTF0);				
					uint8_t Bit = (((PhyPortRX) & PIN)) ? 0 : 1 << NumBitCounter;
					if(Bit != 1)
					{
						//Jam recv
						LastError = COLLISIONDETECTED;
					}

					
					// to stop the timer
#ifdef USE_TIMER0
					TCCR0 = 0;
					TIMSK &= ~(1 << TOIE0);
#else 	
					TIMSK &= ~(1 << OCIE1A);
					TCCR1B = 0;
#endif

					if(receivedByte != NULL) {
						PhyPortTX |= 8;
						PhyPortTX &= ~8;

						receivedByte(Frame);
						// clear INT0
						GIFR |= (1<<INTF0);
						// Enable INT0
						GICR |= 1<<INT0;
				
						if(stopListeningMode > 0) {
							// clear INT0
							GIFR |= (1<<INTF0);
							// Enable INT0
							GICR |= 1<<INT0;
							// handler entfernen
							receivedByte = NULL;
						}
					} else {
						addEntry(Buffer, Frame);
					}
					CurrentState = CS_FINISHED;
					break;	
			}
			break;
	}
	
}



