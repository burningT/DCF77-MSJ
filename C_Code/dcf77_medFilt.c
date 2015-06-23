/* medFilt.c
Gibt den Median der Elemente aus input zur�ck.

Parameter:
input : Eingangsvektor mit Geschwindigkeitswerten.
R�ckgabewert: Geschwindigkeitswert.
*/
#include "dcf77_includes.h"
float dcf77_medFilt(float * input){
	short i;
	/*Sortiere die Geschwindigkeitswerte.*/
	dcf77_sortBub(input, MEDLEN);
	/* Gebe den Median zur�ck.*/
	return input[(short)(MEDLEN / 2)];
}
