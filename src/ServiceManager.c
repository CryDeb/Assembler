#include "ServiceManager.h"

remoteJob Services[MAX_SERVICES];


/*******************************************************************************
* Funktion:  addService 
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Beschreibung:
*	Fügt einen service hinzu
*
* Postcondition:
*	Der Service ist aktiv
*
* Daten:
*	CommandNr	Kommandonummer des Services 
*	Job			Funktionspointer zum Job	
*
*******************************************************************************/
void addService(uint8_t CommandNr, remoteJob Job) {
	if(CommandNr < MAX_SERVICES)
		Services[CommandNr] = Job;

}


/*******************************************************************************
* Funktion:  removeJob 
*									   									   
* Autor:     KS / DW					  						   								   									   
* Version:   1.0				   			   								   									   
* Datum: 	 27.05.2014  							   									   
********************************************************************************
* Beschreibung:
*	Entfernt einen service
*
* Postcondition:
*	Der entfernte Service wird nicht mehr abgehandelt
*
* Daten:
*	CommandNr	zu entfernendes Serive
*
*******************************************************************************/
void removeService(uint8_t CommandNr) {
	if(CommandNr < MAX_SERVICES)
		Services[CommandNr] = 0;
}

void initJobs() {
	addService(0,printToPortC);
}
