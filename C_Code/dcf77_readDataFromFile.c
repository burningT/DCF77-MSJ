/* readDataFromFile.c
Liest short Werte aus der Datei fileName und legt diese, an der Stelle auf die *pCH zeigt, hintereinander ab.

Parameter:
short ** pCH : Call by Reference. Pointer auf den Speicherbereich an den die Werte abgelegt werden.
char fileName [] : Dateiname, aus dem die Werte gelesen werden sollen.

Rückgabewert : Anzahl der Werte in der Datei.
 */
 
#include "dcf77_includes.h"

#ifdef SIMU
unsigned int dcf77_readDataFromFile(short ** pCH, char fileName []) {
	FILE * pFile;
	unsigned int lines = 0;
	pFile = fopen(fileName, "r");
	if (pFile != NULL) {
		int ch;
		unsigned int i;
		/* Zähle Zeilen.*/
		while (EOF != (ch = fgetc(pFile))) {
			if (ch == '\n') {
				++lines;
			}
		}
		/* Zurück zum Anfang des Files*/
		fseek(pFile, 0, SEEK_SET);
		/* Allokiere Speicher, an den die Werte geschrieben werden.*/
		*pCH = (short*) malloc(lines * sizeof(short));
		if (*pCH == NULL) {
			fprintf(stderr,"Fehler beim Speicher allokieren fuer %s\n",fileName);
			return 0;
		}
		/* Lese Zeile für Zeile ein.*/
		for (i = 0; i < lines; i++) {
			char sBuff[MAXNUMCHAR];
			fgets(sBuff, MAXNUMCHAR, pFile);
			if (ferror(pFile)) {
				fprintf(stderr,"Fehler beim lesen aus der Datei %s\n",fileName);
				return 0;
			}
			(*pCH)[i] = (short) atoi(sBuff);
		}
		fclose(pFile);
	}
	return lines;
}
#endif
