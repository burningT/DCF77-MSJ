/* medFilt.c
Gibt den Median der Elemente aus input zur�ck.

Parameter:
input : Eingangsvektor mit Geschwindigkeitswerten.
R�ckgabewert: Geschwindigkeitswert.
*/
#include "dcf77_includes.h"
float dcf77_medFilt(float * input){
	short i;
	float tmp [MEDLEN];
	/*Kopiere Werte vor dem sortieren in ein Zwischenarray. Ansonsten wird
	beim n�chsten berechneten Geschwindigkeitswert, nicht der �lteste,
	sondern der Gr��te entfernt. Weil das Array sortiert bleibt, da Call
	by Reference.*/
	for (i=0;i<MEDLEN;i++){
		tmp[i]=input[i];
	}
	/*Sortiere die Geschwindigkeitswerte.*/
	dcf77_sortBub(tmp,MEDLEN);
	/* Gebe den Median zur�ck.*/
	return tmp[(short)(MEDLEN/2)];
}
