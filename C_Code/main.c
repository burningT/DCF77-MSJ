/* main.c
Entwicklung eines Systems zur optischen Geschwindigkeitsmessung auf einem
 digitalen Signalprozessor. Top-Level-Funktion.
*/

#include "includes.h"

/* Globale Variablen*/
/* Zeigt auf den aktuellen Buffer indem Berechnungen durchgeführt werden sollen */
Complex *pWorkingBuffer;
Complex *pWorkingBufferCH2;

/* Zeigt auf den aktuellen Buffer indem neue Daten gespeichert werden sollen */
Complex *pInputBuffer;
Complex *pInputBufferCH2;

/* Jeweils ein Buffer pro Kanal. Das zweite Paar dient dazu, dass in einem
 Bufferpaar Berechnungen durchgeführt werden können, während das andere
 bereits wieder mit Daten befüllt wird.*/
Complex ABuffer[N];
Complex ABufferCH2[N];
Complex BBuffer[N];
Complex BBufferCH2[N];

#ifdef SHOWCORR
/* Speicher zur Ansicht der letzten Berechneten Kreuzkorrelation.*/
float rxy[N];
#endif

/* Sobald startCalc 1 wird, ist ein Bufferpaar voll und es wird begonnen einen
 Geschwindigkeitswert zu berechnen. Anschließend wird die Variable wieder auf
 0 gesetzt. */
short startCalc = 0;
/* Buffer fuer Speicherung und Darstellung des Geschwindigkeitsverlaufs*/
float veloc [V];
/* Index fuer die Verwendung des Geschwindigkeitsbuffers*/
short iVelo=0;

#ifdef ECHO
/* Speicher zur Ausgabe über die DACs.*/
short inL=0, inR=0;
#endif

#ifdef HARDWARE
/* Zähler für die Anzahl der Werte die in die Buffer geladen wurden.*/
short count_in = 0;
#endif

/*Anlegen des Speichers fuer die Variablen der FFT. Es ist erforderlich
 das diese zusammenhaengend im Speicher liegen um eine gute Performance
 zu erreichen. Dies wird durch die pragma DATAALIGN() Direktive erreicht.
 Macht es allerdings ebenfalls erforderlich, dass die Variablen Global
 angelegt werden. Da diese allerdings ohnehin recht groß sind, ist der
 Umstand das diese dadurch im Heap gespeichert werden zu
 begrüssen. [Chassing S.299]*/
/* Array für die Twiddle Faktoren der FFT*/
Complex w[N / RADIX];
/* indexvariable fuer Schleifen */
unsigned short i = 0;
/* Indices für die Neuordnung der Twiddlekonstanten w*/
short iTwid[N / RADIX];
/* Indices für die Neuordnung der inversen FFT*/
short iFFT[N];
/* Erzeuge Adaptive Filter Strukturen*/
adapFilt af1;
adapFilt af2;
/* Zwischenspeicher für Geschwindigkeiten vor dem Median Filter*/
float vUnfilt [MEDLEN];

#ifdef HARDWARE
/* Sorgt dafür, dass die Variablen im SBSRAM gespeichert werden.*/
#pragma DATA_SECTION(ABufferCH2, ".sbsram")
#pragma DATA_SECTION(BBufferCH2, ".sbsram")
#pragma DATA_SECTION(veloc, ".sbsram")
/* Sorgt dafür, dass die Variablen als komplexe Wertepaare gespeichert
 werden. Mit stets dem Imaginärteil hinter dem dazugehörigen Realteil.*/
#pragma DATA_ALIGN(w,sizeof(Complex))
#pragma DATA_ALIGN(ABuffer,sizeof(Complex))   
#pragma DATA_ALIGN(ABufferCH2,sizeof(Complex))   
#pragma DATA_ALIGN(BBuffer,sizeof(Complex))   
#pragma DATA_ALIGN(BBufferCH2,sizeof(Complex))
#endif

#ifdef INCLUDEDATA
/* Lese Daten aus .h Dateien ein. Die .h Dateien müssen im selben Ordner
 wie die Quellcodes liegen.*/
short inputDataCH1[]={
#include "inputDataCH1.h"
};
short inputDataCH2[]={
#include "inputDataCH2.h"
};
float resultData[]={
#include "resultData.h"
};
#pragma DATA_SECTION(inputDataCH1, ".sbsram")
#pragma DATA_SECTION(inputDataCH2, ".sbsram")
#pragma DATA_SECTION(resultData, ".sbsram")
#endif

#ifdef SHOWCORR
#pragma DATA_SECTION(rxy, ".sbsram")
#endif

/* Beginn Main()*/
int main(void) {

#ifdef SIMU	
	/* Pointer auf Datensätze.*/
	short *pXn = NULL,*pYn = NULL;
	/* Pointer für Datenätze nach Konvertierung nach float.*/
	float *pDataXn=NULL, *pDataYn=NULL;
	/* Index für das aktuelle Element im Datensatz. */
	unsigned int dataIndex=0;
	/* Zähler für die Anzahl der Werte im Datensatz. */
	unsigned int lineCountXn = 0, lineCountYn = 0;
	
	/* Erstelle leere Dateien zur Ausgabe, überschreibe alte wenn vorhanden.*/
	FILE * pFile;
	pFile = fopen("x_n_s_proc.txt", "w"); fclose(pFile);
	pFile = fopen("y_n_s_proc.txt", "w"); fclose(pFile);
	pFile = fopen("x_n_s_filt_proc.txt", "w"); fclose(pFile);
	pFile = fopen("y_n_s_filt_proc.txt", "w"); fclose(pFile);
	pFile = fopen("corr1_proc.txt", "w"); fclose(pFile);
	pFile = fopen("v_proc.txt", "w"); fclose(pFile);
	
	/* Datensätze aus Datei einlesen */
	lineCountXn = readDataFromFile(& pXn,"x_n_s.txt");
	if (pXn == NULL) {
		return EXIT_FAILURE;
	}
	lineCountYn = readDataFromFile(& pYn,"y_n_s.txt");
	if (pYn == NULL) {
		return EXIT_FAILURE;
	}
	/* Datensätze nach float konvertieren */
	pDataXn = sshortToFloat(pXn,lineCountXn);
	pDataYn = sshortToFloat(pYn,lineCountYn);
#endif

#ifdef HARDWARE	
	/* Initialisiere DSP-Board und Codec.*/
	initHardware();
#endif

	/* Initialsierung der Datenbufferpointer.*/
	pInputBuffer = ABuffer;
	pInputBufferCH2 = ABufferCH2;
	pWorkingBuffer = BBuffer;
	pWorkingBufferCH2 = BBufferCH2;
	/* Initialisierung Zwischenspeicher für Geschwindigkeiten vor dem
	Median Filter.*/
	for(i=0; i < MEDLEN; i++){
		vUnfilt[i]=0.0f;
	}
	
	/* Twiddlekonstanten vorberechnen. Der Sinusterm muss positiv sein
	Aufgrund der Implementierung des FFT Algorithmus von TI. Allgemein
	müsste dieser Aufgrund der Formel negativ
	sein. [cfftr2_dit.asm unter ASSUMPTIONS]*/
	for (i = 0; i < N / RADIX; i++) {
		w[i].re = (float)cos(OMEGA * i);
		w[i].im = (float)sin(OMEGA * i);
	}
	
	/* Erzeuge Indices für Anordnung der Twiddlekonstanten w.*/
	digitrev_index(iTwid, N / RADIX, RADIX);
	/* Führe Anordnung für die Twiddlekonstanten aus.*/
	bitrev(w, iTwid, N / RADIX); 	
	/* Erzeuge Indices für Anordnung der FFT Ergebnisse.*/
	digitrev_index(iFFT, N, RADIX);
	/* Initialisiere Geschwindigkeitsbuffur.*/
	for(i=0;i<V;i++){
		veloc[i]=0.0f;		
	}	
	/* Initialisiere Adaptive Filter.*/
	initAdapFilt(&af1, CH1);
	initAdapFilt(&af2, CH2);
	
#ifdef HARDWARE
	/* Starte Interrupts. */
	IER |= 2;
	CSR |= 1; 
#endif

	/* Berechnungsschleife. */
#ifdef HARDWARE
	for (;;) {
#endif

#ifdef SIMU
		/*Es kann angenommen werden, dass Xn und Yn gleich lang sind*/
		for(; dataIndex<lineCountXn; ) {
			/*Kopiere eingelesene Daten in Buffer. Auf der Hardware	passiert dies im Interrupt.*/
			dataAcquisition(&dataIndex, pDataXn, pDataYn, lineCountXn);
#endif

			/* Buffer voll? Starte Berechnungen*/
			if (startCalc == 1) {
			
#ifdef INCLUDEDATA
				/* Interrupt deaktivieren*/
				CSR &= ~1;  
#endif

#ifdef SIMU
				/* Datensaetze zur Verifikation in Datei schreiben.*/
				writeCompReToFile(pWorkingBuffer,BUFLEN,"x_n_s_proc.txt");			
				writeCompReToFile(pWorkingBufferCH2,BUFLEN,"y_n_s_proc.txt");				
#endif

				/* Wende adaptive Filter an.*/
				applyFilter(&af1, &af2);
				
#ifdef SIMU				
				writeCompReToFile(pWorkingBuffer,BUFLEN,"x_n_s_filt_proc.txt");
				writeCompReToFile(pWorkingBufferCH2,BUFLEN,"y_n_s_filt_proc.txt");
#endif

				/*Berechne Kreuzkorrelation.*/
				calcCorr(iFFT);
				
#ifdef INCLUDEDATA						
				/* Vergleiche Korrelationsergebnisse.*/			
				compCorr();
				send_string("Vergleich Korrelation abgeschlossen. Programm endet. \r\n");
				return EXIT_SUCCESS;
#endif

#ifdef SIMU				
				writeCompReToFile(pWorkingBuffer,N-1,"corr1_proc.txt");
#endif

				/*Schiebe die letzten MEDLEN-Elemente von vUnfilt um ein Element weiter.
				Das letzt Element fällt herraus und das Nullte Element wird durch einen
				neu berechneten Wert ersetzt.*/
				for(i=0;i<MEDLEN-1;i++){
					vUnfilt[MEDLEN-1-i] = vUnfilt[MEDLEN-2-i];
				}
				vUnfilt[0] = calcVelo();
				/* Wende Medianfilter an und speichere den Wert in Geschwindigkeitsarray ab.*/
				veloc[iVelo]=medFilt(vUnfilt);
				iVelo++;
				if(iVelo >= V){
					iVelo=0;
				}
				/* Setzte die nächsten 5 Elemente auf Null um eine schönere grafische
				Ausgabe zu erhalten*/
				for(i=0;i<5;i++){
					veloc[(iVelo+i)%V]=0;
				}
				
#ifdef HARDWARE
				/*Gebe den aktuellen Geschwindigkeitswert über das Terminal aus.*/
				send_string(stoa((short)(veloc[iVelo-1]*SCALETERMINAL)));
#endif

#ifdef SIMU
				writeDataToFile(&(veloc[iVelo-1]),1,"v_proc.txt");
#endif
				//Berechnungen abgeschlossen
				startCalc = 0;
			}
		}
		
#ifdef SIMU
		/*Berechnung abgeschlossen. Gebe allokierten Speicher frei.*/
		free(pDataXn); pDataXn=NULL;
		free(pDataYn); pDataYn=NULL;
		puts("!!!Berechnung abgeschlossen!!!");
		system("pause");
		return EXIT_SUCCESS;
#endif
	}
	

