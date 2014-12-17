#include <avr/io.h>
#include <stdint.h>
#include "FrameLayer.h"
#include "PhyLayer.h"
#include "ServiceManager.h"
//temporäres frame um daten einzulesen
frame_t LastFrame;
//frame das auf verarbeitung wartet
frame_t PendingFrame;
//falg gibt an ob ein Frame verarbeitet werden kann
uint8_t Pending;
//aktuller receive status, gibt an welches byte als nächste eingelesen wird 
static RecvState_t CurrentState = Destination;
// flag ob packet ignoriert werden soll
uint8_t Ignore;
// einzulesende paramter
uint8_t ParameterLength;
uint8_t ParameterIndex;
/*******************************************************************************/

void initLayer() {
	CurrentState = Destination;
	Pending = FALSE;
}

/*******************************************************************************
* Funktion:  sendFrame
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: sendFrame(${VERWEISS AUF FRAME});
*
* HINWEIS: ist nich vollständig und nur ein Prototyp
*
* Beschreibung:
*	Versendet das übergebene Frame byte-weise. Nach dem versand wird auf Be-
*	stätigung gewartet.
*
* Daten:
*	frame_t* Frame	zu verschickendes Frame
*
*******************************************************************************/
uint8_t sendFrame(frame_t *Frame) {
	char Counter;
	uint8_t BCC = 0;
	char *IterPtr = (char *)Frame;
	uint8_t Elements = NumFrameElements;
	
	//do {
		for(Counter = 0; Counter < Elements; Counter++, IterPtr++) { 
			if(Counter == Parameters) {
			
				IterPtr = Frame->Parameters;
				// get number of parameters 
				Elements += *IterPtr;

			}
			putByte(*IterPtr);
		
			BCC ^= *IterPtr;
		}

		putByte(BCC);
	
	//} while(getByte(TRUE) == NAK);
}
/*******************************************************************************
* Funktion:  receiveFrame
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: receiveFrame(${EMPFANGENES BYTE});
*
*
* Beschreibung:
*	Handler für Byte-Empfang. Liesst Byte-weise das Frame ein und verwaltet den 
*	Zustand über ein State-machine
*	
*
* Daten:
*	uint8_t Byte 	Einglesenes Byte
*
*******************************************************************************/
void receiveFrame(uint8_t Byte) {
	switch(CurrentState) {
		case Destination:
			if(Byte != LocalAddress || Pending == TRUE) {
				Ignore  = 1;
			} else {
				Ignore = 0;
			}
			ParameterLength = ParameterIndex = 0;
			LastFrame.CheckSum = 0;
			LastFrame.Destination = Byte;
			CurrentState = Source;
			break;
		case Source:
			LastFrame.Source = Byte;
			CurrentState = SeqNumL;
			break;
		case SeqNumL:
			LastFrame.SequenceNumL = Byte;
			CurrentState = SeqNumH;
			
			break;
		case SeqNumH:
			LastFrame.SequenceNumH = Byte;
			CurrentState = CommandNum;
			
			break;
		case CommandNum:
			LastFrame.CommandNumber = Byte;
			CurrentState = Parameters;
			break;
		case Parameters:
			if(ParameterLength == 0) {
				ParameterLength = Byte;
				LastFrame.Parameters = (char *)malloc(ParameterLength);
			} 
			
			LastFrame.Parameters[ParameterIndex++] = Byte;				
			if(ParameterLength == ParameterIndex)
				CurrentState = CheckSum;
			break;
		case CheckSum:
			if(Ignore == 0) {
				if(LastFrame.CheckSum == Byte && Pending == FALSE) {
					putByteAsync(ACK, TRUE);
					memcpy(&PendingFrame,&LastFrame, sizeof(frame_t));
					Pending = TRUE;
				
				} else {
					putByteAsync(NAK, TRUE);
				}
			}
			CurrentState = Destination;
			break;
	}
	LastFrame.CheckSum ^= Byte;
}
/*******************************************************************************
* Funktion:  handleIncomingTask
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: handleIncomingTask()
*
*
* Beschreibung:
*	Verarbeite wartendes Kommando/Frame falls vorhanden.
*
* Precondition: 
*	Die Kommandos müssen registriert sein.
*	
*******************************************************************************/
void handleIncomingTask() {
	if(Pending) {
		Services[PendingFrame.CommandNumber](&PendingFrame);
		free(PendingFrame.Parameters);
		Pending = FALSE;
	}
}
