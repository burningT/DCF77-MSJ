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

#include "includes.h"

#ifdef SIMU
extern Complex * pWorkingBufferCH1;
extern Complex * pWorkingBufferCH2;
extern Complex * pInputBufferCH1;
extern Complex * pInputBufferCH2;
extern short startCalc;

void dataAcquisition(unsigned int * dataIndex, float * xn, float * yn, unsigned int lineCount) {
	unsigned int i = 0;
	/* Kopiere BUFLEN Werte*/
	for (i = 0; i < BUFLEN; i++) {
		if (*dataIndex < lineCount){
			pInputBufferCH1[i].re = xn[*dataIndex];
			pInputBufferCH2[BUFLEN-1-i].re = yn[*dataIndex];
		}
		else{
			/* Alle Datens�tze verarbeitet, f�lle mit Nullen auf.*/
			pInputBufferCH1[i].re = 0.0f;
			pInputBufferCH2[BUFLEN-1-i].re = 0.0f;
		}
		/* Die imagin�r-Anteile sind Null;*/
		pInputBufferCH1[i].im = 0.0f;			
		pInputBufferCH2[BUFLEN-1-i].im = 0.0f;
		(*dataIndex)++;
	}	
	/* Tausche Input- und Workingbuffer.*/
	swapCompPoint(&pInputBufferCH1, &pWorkingBufferCH1);
	swapCompPoint(&pInputBufferCH2, &pWorkingBufferCH2);
	/* Berechnung starten.*/
	startCalc = 1;
	return;
}
#endif
