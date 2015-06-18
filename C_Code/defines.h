/* defines.h
Enth�lt die gesammelten Symbolischen Konstanten.
 */
 
#ifndef DEFINES_H_
#define DEFINES_H_
/*Abtastfrequenz*/
#define FSAMP   8000 
/* Messfensterl�nge/ Bufferl�nge f�r die ADCs. D.h. Wie viele Punkte werden
 eingelesen, bevor eine neue Berechnung gestartet wird */
#define BUFLEN	2048
/* Sensorabstand in m*/
#define DIST 	0.01f 
/* Bufferl�nge f�r die berechneten Geschwindigkeitswerte*/
#define V 10000
/* Anzahl der Punkte bei der FFT um der linearen Faltung zu entsprechen,
 entspricht der Gr��e der Buffer zum halten der Daten.*/
#define N 		(2*BUFLEN)	
/* Radix der FFT */
#define RADIX	2			
/* Kreiszahl pi*/
#define PI      3.141592654f
/* Zur Berechnung der Twiddlefaktoren*/
#define OMEGA	((2*PI)/N) 
/* Konvertierungsfaktor short zu float*/
#define SHOTOFLO 32768
/* Skalierungsfaktor, notwendig da beide Sensoren leicht unterschiedliche
 Pegel liefern.*/  
#define SCALECH 1.1f
/* Kanalbezeichner*/
#define CH1 1
#define CH2 2

/* Parameter adaptives Filter*/
/* L�nge des Filters*/
#define L 95
/* Verz�gerung in Abtastschritten.*/
#define D 40
/* Schrittweite */
#define MUN 0.03f
/* Konstante zur Verhinderung ein Division durch 0. */
#define EPS 0.001f 

/* Median Filter L�nge*/
#define MEDLEN 5
#ifdef SIMU
/*Wie viele chars werden aus der Textdatei gelesen */
#define MAXNUMCHAR 15
#endif
#ifdef HARDWARE
/* Skalierungsfaktor f�r die Ausgabe �ber das Terminal. */
#define SCALETERMINAL 1000
#endif
#ifdef INCLUDEDATA
/*Schwelle zum vergleichen der Korrelationsergebnisse*/
#define COMPTHRESHHOLD 0.000001f
#endif
#endif /* DEFINES_H_ */
