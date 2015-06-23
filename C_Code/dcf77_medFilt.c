/* medFilt.c
Gibt den Median der Elemente aus input zurück.

Parameter:
input : Eingangsvektor mit Geschwindigkeitswerten.
Rückgabewert: Geschwindigkeitswert.
*/
#include "dcf77_includes.h"
float dcf77_medFilt(float * input){
	short i;
	/*Sortiere die Geschwindigkeitswerte.*/
	dcf77_sortBub(input, MEDLEN);
	/* Gebe den Median zurück.*/
	return input[(short)(MEDLEN / 2)];
}
