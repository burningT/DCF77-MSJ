/* writeDataToFile.c
Die Funktion schreibt den Speicherbereich auf den var zeigt in die .txt-Datei
fileName als int Werte. Die Operation wird abgebrochen, sobald numOfElements
Elemente geschrieben wurden.

Parameter:
int * var: Pointer auf int Array, dass in die Datei geschrieben werden soll.
unsigned int numOfElements : Die Anzahl der Elemente die geschrieben werden sollen.
char * fileName : Dateiname, in den das Array geschrieben werden soll. 
 */

#include "dcf77_includes.h"

#ifdef SIMU

void dcf77_writeDataToFile(short * var, unsigned int numOfElements, char * fileName) {
	FILE * pFile = fopen(fileName, "a");
	unsigned int i;
	if (pFile != NULL) {
		for (i = 0; i < numOfElements; i++) {
			fprintf(pFile, "%d\n", *var);
			if (ferror(pFile)) {
				fprintf(stderr, "Fehler beim schreiben in Datei %s\n", fileName);
				return;
			}
			var = var + 1;
		}
		fclose(pFile);
	} else {
		fprintf(stderr, "Fehler beim schreiben in Datei; Filename : %s \n",	fileName);
	}
	return;
}
#endif
