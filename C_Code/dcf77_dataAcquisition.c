/* dataAcquisition.c
Bef�llt in der Simulation die Buffer mit den Daten aus den Datens�tzen.
 Es werden bei jedem Aufruf BUFLEN Werte f�r beide Kan�le kopiert. F�r
 Kanal 2 werden die Werte bereits hier umgekehrt in den Buffer kopiert,
 dies erlaubt ein einfacheres berechnen der Korrelation.
 
Parameter:
unsigned int * dataIndex : Index des n�chsten, nicht verwendeten,
 Datenpunktes im Datensatz.
float * xn : Eingangsignal Kanal 1.
float * yn : Eingangssignal Kanal 2.
unsigned int lineCount : Anzahl der Elemente im Datensatz
*/

#include "dcf77_includes.h"

#ifdef SIMU
extern Complex * pWorkingBuffer;
extern Complex * pInputBuffer;
extern short startCalc;

void dcf77_dataAcquisition(unsigned int * dataIndex, float * xn, unsigned int lineCount) {
	unsigned int i = 0;
	/* Kopiere BUFLEN Werte*/
	for (i = 0; i < BUFLEN; i++) {
		if (*dataIndex < lineCount){
			pInputBuffer[i].re = xn[*dataIndex];
		}
		else{
			/* Alle Datens�tze verarbeitet, f�lle mit Nullen auf.*/
			pInputBuffer[i].re = 0.0f;
		}
		/* Die imagin�r-Anteile sind Null;*/
		pInputBuffer[i].im = 0.0f;	
		(*dataIndex)++;
	}	
	/* Tausche Input- und Workingbuffer.*/
	dcf77_swapCompPoint(&pInputBuffer, &pWorkingBuffer);
	/* Berechnung starten.*/
	startCalc = 1;
	return;
}
#endif
