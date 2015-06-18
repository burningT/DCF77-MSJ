/* dcf77_defines.h
Enthält die gesammelten Symbolischen Konstanten.
*/

#ifndef DCF77_DEFINES_H_
#define DCF77_DEFINES_H_

#if defined(HARDWARE) || defined(SIMU)

//#define TI_COMPILER_USED				//TR: was stimmt hier nicht?

#endif /*!defined(HARDWARE) && !defined(SIMU)*/

#if defined(SIMU)
/*fopen,scanf,..  Warning deaktivieren im MVS Compiler*/
#define _CRT_SECURE_NO_WARNINGS
/*Wie viele chars werden aus der Textdatei gelesen */
#define MAXNUMCHAR 15

#endif



#define DATE_TIME_STRING_LEN 50

/*Abtastfrequenz*/
#define FSAMP   5438.596f 
/* Messfensterlänge/ Bufferlänge für die ADCs. D.h. Wie viele Punkte werden
eingelesen, bevor eine neue Berechnung gestartet wird */
#define BUFLEN	4096
/* Anzahl der Punkte bei der FFT um der linearen Faltung zu entsprechen,
entspricht der Größe der Buffer zum halten der Daten.*/
#define N 		(2*BUFLEN)	
/* Radix der FFT */
#define RADIX	2			
/* Kreiszahl pi*/
#define PI      3.141592654f
/* Zur Berechnung der Twiddlefaktoren*/
#define OMEGA	((2*PI)/N) 
/* Konvertierungsfaktor short zu float*/
#define SHOTOFLO 32768
/* Bufferlänge für die berechneten Korrelationsergebnisse*/
#define V 1000
/* Signalverstärkungsfaktor*/
#define AMPLIFIE 8
/* Median Filter Länge*/
#define MEDLEN 7
#endif /* DCF77_DEFINES_H_ */
