/* calcVelo.c
Gibt den Index des Maximums aus dem Korrelationsergebnis zurück

Rückgabewert:
Der berechnete Index des Maximums.
*/
#include "dcf77_includes.h"
extern Complex * pPLLWorkingBuffer;
short dcf77_searchMax(){
	short i=0;
	short iMax=0;
	float valueMax=0;
	float Tv=0;
	/* Es werden zwar 2*BUFLEN Werte übergeben, es sollen nun aber nurnoch
	2*BUFLEN-1 Werte interpretiert. Dies entspricht dem erwarteten Ergebnis
	einer Korrelation basierend auf der linearen	Faltung. Dies entspricht
	der Grenze <N-1. Innerhalb dieser Grenzen wird das Maximum gesucht.*/
	for(i=0;i<N-1;i++){
		if(valueMax < abs(pPLLWorkingBuffer[i].re)){
			valueMax = abs(pPLLWorkingBuffer[i].re);
			iMax = i;
		}
	}
	/* Index so umrechnen, dass ein Maximum in der Mitte, also wenn keine
	Verzögerung vorliegt, dem Wert 0 entspricht.*/
	iMax= iMax-(BUFLEN-1);
	
	return iMax;	
}
