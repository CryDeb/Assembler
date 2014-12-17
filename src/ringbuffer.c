#include "ringbuffer.h"
#define SUCCESS 0
/*******************************************************************************
* Funktion:  createRingBuffer
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: createRingBuffer(${GROESSE DES BUFFER});
*
* Beschreibung:
*	Erzeugt und initialisiert einen Ringbuffer f�r die angegebene Gr�sse.
*
*
* Postcondition:
*	Speicher wird f�r den Ringbuffer alloziert. Dieser muss wider durch 
*	releaseRingBuffer freigegeben werden.
*	
*
* Daten:
*	uint8_t Size	anzahl der m�glicher speicherzellen im ringbuffer
*******************************************************************************/
RingBuffer *createRingBuffer(uint8_t Size) {
	RingBuffer *Buffer = (RingBuffer *)malloc(sizeof(RingBuffer));
	if(Buffer != NULL) {
		Buffer->Data =  (uint8_t*)malloc(sizeof(uint8_t)*Size);
		if(Buffer->Data != NULL) {
			Buffer->Size = Size;
			clearRingBuffer(Buffer);
		} else {
			free(Buffer);
			Buffer = NULL;
		}
	}
	
	return Buffer;
}
/*******************************************************************************
* Funktion:  clearRingBuffer
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: clearRingBuffer(${VERWEISE AUF RINGBUFFER});
*
* Beschreibung:
*	Setzt den Zustand des Ringbuffer zur�ck, Daten im Ringbuffer gehen verloren.
*
* Precondition: 
*	Speicher f�r Ringbuffer muss alloziert sein
*	
*
* Daten:
*	RingBuffer*	Buffer	Verweis auf den Ringbuffer
*
*******************************************************************************/
void clearRingBuffer(RingBuffer *Buffer) {
	if(Buffer != NULL) {
		Buffer->Sentinel = 0;
		Buffer->CurrentEntry = 0;
		Buffer->Counter = 0;
	}
}
/*******************************************************************************
* Funktion:  releaseRingBuffer
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: releaseRingBuffer(${VERWEISE AUF RINGBUFFER});
*
* Beschreibung:
*	Gibt den alloziwrten Spiecher des Ringbuffer wieder frei.
*
* Precondition: 
*	Speicher f�r Ringbuffer muss alloziert sein	
*
* Postcondition:
*	Verweis auf Ringbuffer geht verloren, er kann nicht mehr ben�tzt werden.
*	
*
* Daten:
*	RingBuffer*	Buffer	Verweis auf den Ringbuffer
*
*******************************************************************************/
void releaseRingBuffer(RingBuffer *Buffer) {
	if(Buffer != NULL) {
		if(Buffer->Data != NULL)
			free(Buffer->Data);
		free(Buffer);
	}
	Buffer = NULL;
}
/*******************************************************************************
* Funktion:  addEntry
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: addEntry(${VERWEISE AUF RINGBUFFER},${WERT});
*
* Beschreibung:
*	F�gt den Wert hinzu, falls gen�gend Platz vorhanden ist. Wenn kein Platz vor-
*	handen ist wird ein OVERFLOW Error zur�ckgegeben.
*
* Precondition: 
*	Speicher f�r Ringbuffer muss alloziert sein	
*
*	
*
* Daten:
*	RingBuffer*	Buffer	Verweis auf den Ringbuffer
*	uint8_t	Byte	Wert der hinzugef�gt wird
*	int16_t	RETURN 	Errorcode
*
*******************************************************************************/
int16_t addEntry(RingBuffer *Buffer, uint8_t Byte) {
	uint16_t Error = SUCCESS;
	if(Buffer != NULL && Buffer->Data != NULL) {			
		if(Buffer->Counter <= Buffer->Size) {
			*(Buffer->Data + Buffer->Sentinel) = Byte;
			Buffer->Sentinel = (Buffer->CurrentEntry + 1) % Buffer->Size;		
			Buffer->Counter++;
		} else {
			// overflow sentinal + 1 will be currententry index
			Error = OVERFLOW;
		}
	} else {
		Byte == INVALIDRBREF;
	}
	return Error;
}
/*******************************************************************************
* Funktion:  getEntry
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: getEntry(${VERWEISE AUF RINGBUFFER});
*
* Beschreibung:
*	Getter f�r aktuellen Wert aus Ringbuffer. Falls der Buffer leer ist wird
*	BUFFEREMPTY zur�ckgegeben.
*
* Precondition: 
*	Speicher f�r Ringbuffer muss alloziert sein		
*	
*
* Daten:
*	RingBuffer*	Buffer	Verweis auf den Ringbuffer
*
*******************************************************************************/
int16_t getEntry(RingBuffer *Buffer) {
	int16_t Byte = BUFFEREMPTY;
	if(Buffer != NULL && Buffer->Data != NULL) {
		if(Buffer->Counter > 0) {
			Byte = *(Buffer->Data + Buffer->CurrentEntry);
			Buffer->CurrentEntry++;
			Buffer->CurrentEntry %= Buffer->Size;
			Buffer->Counter--;
			
		} 
	} else {
		Byte == INVALIDRBREF;
	}
	return Byte;
}
