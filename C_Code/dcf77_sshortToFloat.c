/* sshortToFloat.c
 Konvertiert die count short Werte beginnend mit pIn in float Werte
 beginnend und speichert diese an der Stelle auf die pData zeigt.
 Wird verwendet für die Simulation mit Daten aus einer Datei, um
 den ausgelesenen Datensatz mit signed short Werten als float Werte
 zwischen zu speichern.
 
Parameter:
pIn: Pointer auf die Speicherstelle an der die short Werte abgelegt
 sind, die konvertiert werden sollen.
count: Anzahl der Werte die konvertiert werden sollen.
Rückgabewert: Pointer auf die Speicherstelle an der die float Werte
 abgelegt sind.
 */
 
#include "dcf77_includes.h"
#ifdef SIMU

float * dcf77_sshortToFloat(short * pIn, unsigned int count) {

	/* Allokiere Speicher für die float Werte.*/
	float * pData = (float*) malloc(count * sizeof(float));
	unsigned int i = 0;
	if (pData == NULL) {
		fprintf(stderr,
				"Fehler beim Speicher allokieren fuer die Konvertierung"
				" von short nach float \n");
		return NULL;
	}
	/* Nehme Konvertierung vor. Die Werte werden durch SHOTOFLO geteilt,
	dadurch liegen diese näher an Null und erhalten dadurch eine höhere
	Genauigkeit.*/
	for (i = 0; i < count; i++) {
		pData[i] = ((float) (pIn[i])) / SHOTOFLO;
	}	/*Speicher fuer short wieder freigeben*/
	free(pIn);	pIn = NULL;	
	return pData;
}
#endif
