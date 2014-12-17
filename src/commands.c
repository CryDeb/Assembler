#include <stdint.h>
#include <stddef.h>
#include "menu.h"
#include "commands.h"
#include "display.h"
//externen variablen
extern settings AppSettings;
extern uint8_t HH, MM, SS;
extern uint8_t inputRepeat;
/*******************************************************************************
* Beschreibung:
*	Hier werden alle Menu command definiert, diese können als handler für 
*	up, down, right, left und enter dienen
*******************************************************************************/

/*******************************************************************************
* Funktion:  go2Field
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: go2Field(${POSITION});
*
* Beschreibung:
*	Wechseln der Feldes/Cursors nach Pos (auf der geleichen Ebenen), falls kein 
*	Feld auf der angegebenen Position vorhande ist, wird nichts getan
*
*
* Daten:
*	uint8_t Pos		Position des Feldes, die unteren 6 bits stellen die X und die 
*					oberen 2 bits die Y Koordinate dar
*
*******************************************************************************/
void go2Field(uint8_t Pos) {
    if(inputRepeat){
	field *Field = AppSettings.currentLayer->Fields[0];	
	uint8_t PosX = Pos & 0x3F;
	uint8_t PosY = (Pos & 0xC0) >> 6;
		for (uint8_t count = 0; count<NUMBEROFUSEDINPUTS; count++){
		    Field = AppSettings.currentLayer->Fields[count];
		    if(Field->Pos.PosX == PosX && Field->Pos.PosY == PosY){
				AppSettings.Status = 4;
				AppSettings.currentField = Field;
		    }
		}
    }
}
/*******************************************************************************
* Funktion:  go2Layer
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: go2Layer(${LayerId})
*
* Beschreibung:
*	Wechseln der Ebenen. Der Courser wird auf das erste definierte Feld gesetzt.
*	Falls der Layer mit der angegeben Id nicht vorhanden ist wird nichts uter-
*	nommen
*
* Daten:
*	uint8_t LayerId	Id des neuen Layers
*
*******************************************************************************/
void go2Layer(uint8_t LayerId){
	uint8_t Index = 0;
	layer *Layers = AppSettings.Layers[Index];	
	while(Layers != NULL && Layers->Id != LayerId){
		Index++;
		Layers = AppSettings.Layers[Index];
	}

	if(Layers != NULL){
		LCD_CLR();
		AppSettings.currentLayer = Layers;
		AppSettings.Status = 7;
		// first field is entry point
		AppSettings.currentField = Layers->Fields[0];
	}
}
/*******************************************************************************
* Funktion:  changeNumber
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: changeNumber
*
* Beschreibung:
*	Inkrementiert die Zeit(auflösungen) 
*
* Daten:
*	uint8_t	changeValue		Gibt an ob Stunden, Minuten oder Sekunden gewechselt
*							werden müssen
*
*******************************************************************************/
void changeNumber(uint8_t changeValue){
    switch(changeValue){
		case 1:
		    changeTimeNumber(&HH, 24);
		    break;
		case 2:
		    changeTimeNumber(&MM, 60);
		    break;
		case 3:
		    changeTimeNumber(&SS, 60);
		    break;
    }
}
/*******************************************************************************
* Funktion:  changeTimeNumber
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Aufruf: changeTimeNumber(${VERWEIS AUF WERT},${MAX WERT});
*
* Beschreibung:
*	Inkrementiert die ${Time} variable falls ${maxVal} erreicht wurde, wird der 
*	Wert auf 0 gesetzt.
*
* Daten:
*	uint8_t* Time	Verweis auf Zeitwert
*	uint8_t	 maxVal	Maximaler Wert für Time
*
*******************************************************************************/
void changeTimeNumber(uint8_t *Time, uint8_t maxVal){
    CUR_OFF();
    CURB_OFF();
    ((*Time)<(maxVal-1))?((*Time)++):((*Time)=0);
    setPosition(getPosition()+1);
    LCD_OutTwoNumbers(*Time);
    setPosition(getPosition()-3);
    CUR_ON();
    CURB_ON();
    
}
