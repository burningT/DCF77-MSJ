/* adda16_adcint.c
Interrupt Handler zur Erfassung der Daten der ADCs.
 */
 
#include "includes.h"

#ifdef HARDWARE
extern short count_in;
extern Complex * pWorkingBuffer;
extern Complex * pWorkingBufferCH2;
extern Complex * pInputBuffer;
extern Complex * pInputBufferCH2;
extern short startCalc;
#ifdef ECHO
extern short inL, inR;
#endif
#ifdef INCLUDEDATA
extern short inputDataCH1[];
extern short inputDataCH2[];
#endif

interrupt void adda16_adcint(void) {

#ifdef ECHO
/* Schreibe im ECHO Modus das alte Eingangssignal an die DACs*/
	ADDA16_WRITE_DAC(0, inL);
	while (!(ADDA16 ->cfg & ADDA16_DAC_READY)){
		;
		ADDA16_WRITE_DAC(1, inR);
	}
#endif

#ifdef INCLUDEDATA
/* Lese die Daten aus dem Datensatz der über die .h-Datei eingelesen wurde. Hier ist bei
 Kanal 1 keine Skalierung notwendig, da dies bereits in der .wav-Datei geschehen ist.*/
	(pInputBuffer[count_in]).re = ((float)(inputDataCH1[count_in]))/SHOTOFLO;
	(pInputBufferCH2[BUFLEN-1-count_in]).re = ((float)(inputDataCH2[count_in]))/SHOTOFLO;
#endif

#ifdef REALTIME
/* Lesen Daten aus den ADCs und speichere in den Buffer. Kanal 2 wird skaliert um
 Pegelunterschiede auszugleichen.*/
	(pInputBuffer[count_in]).re = ((float)(ADDA16_READ_ADC(0)))/SHOTOFLO;
	(pInputBufferCH2[BUFLEN-1-count_in]).re = ((float)(ADDA16_READ_ADC(1)))/SHOTOFLO*SCALECH;
#endif

#ifdef ECHO
/* Speichere im ECHO Modus das Eingangssignal zur Wiederausgabe ab.*/
	inL=(short)(((pInputBuffer[count_in]).re)*SHOTOFLO);
	inR=(short)(((pInputBufferCH2[BUFLEN-1-count_in]).re)*SHOTOFLO);
#endif

/* Die Imaginärteile sind Null.*/
	(pInputBuffer[count_in]).im = 0.0f;
	(pInputBufferCH2[BUFLEN-1-count_in]).im = 0.0f;
/* Sobald der Buffer voll ist, starte Berechnung.*/
	count_in++;
	if (count_in >= BUFLEN){
		count_in = 0;
		/* Tausche Pointer auf Working und Input Buffer*/
		swapCompPoint(&pInputBuffer, &pWorkingBuffer);
		swapCompPoint(&pInputBufferCH2, &pWorkingBufferCH2);	
		/* Prüfe ob die vorherige Berechnung abgeschlossen ist und starte neue Berechnung.*/
		if(startCalc == 1){
			send_string("FEHLER! Berechnung noch nicht fertig. \r\n");
		}else{
			startCalc = 1;		
		}
	}
}
#endif
