/*compCorr.c
Vergleicht das Korrelationsergebnis, welches auf dem DSP berechnet wurde,
 mit dem aus Matlab, über eine .h-Datei, übertragenen Ergebnis. Wird ein
 Unterschied festgestellt, wird eine Fehlermeldung gesendet.*/
 
#include "includes.h"

#ifdef INCLUDEDATA
extern float resultData[];
extern Complex * pWorkingBuffer;

void compCorr(){
	short i;
	short flag=0;
	for(i=0;i<N-1;i++){
		/* Float Werte lassen sich nicht auf Gleichheit überprüfen.
		Deshalb wird eine Schwelle definiert.*/
	if(fabs(resultData[i] - (pWorkingBuffer[i]).re)>COMPTHRESHHOLD){
		flag =1;
		}
	}
	if(flag){
	send_string("Unterschied bei Korrelation festgestellt!\r\n");
	}

	return;
}
#endif

