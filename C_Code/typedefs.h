/* typedefs.h
Enth�lt alle selbst erstellten Datentypen.
*/

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

typedef struct complexNumber {
	float re, im;
} Complex;

typedef struct adaptiveFilter {
	/* Filterparameter*/
	float w [L];
	/* Verz�gertes Filtereingangssignal. Braucht nicht initialisiert
	werden, da es vor jeder Anwendung neu gesetzt wird.*/
	float x [BUFLEN];
	/* Filterausgangswert*/
	float y [BUFLEN];
	/* Speicher Verz�gerungsglieder*/
	float dm [L];
	/* Signalenergie*/
	float se ;
	/* Kanal : Die Kanal abfrage ist notwendig, da Kanal 2 'R�ckw�rts'
	im Buffer gespeichert wird, aber im NLMS Algorithmus deshalb vom
	letzten Element beginnend interpretiert werden muss.*/
	short CH;
} adapFilt;

#endif
