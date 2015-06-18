/* dcf77_main.c
15.06.15 TB: erstellt

Entwicklung eines Systems zum Empfang des DCF77 Signals auf einem
digitalen Signalprozessor. Top-Level-Funktion.
*/

#include "dcf77_includes.h"
/*Hilbert Filter*/
#include "dcf77_hilbert.h"

/* Globale Variablen*/
/* Zeigt auf den aktuellen Buffer indem Berechnungen durchgeführt werden sollen */
Complex *pWorkingBuffer;

/* Zeigt auf den aktuellen Buffer indem neue Daten gespeichert werden sollen */
Complex *pInputBuffer;

/* Jeweils ein Buffer pro Kanal. Der zweite dient dazu, dass in dem
Buffer Berechnungen durchgeführt werden können, während das andere
bereits wieder mit Daten befüllt wird.*/
Complex ABuffer[BUFLEN];
Complex BBuffer[BUFLEN];

float envelope[BUFLEN];

/*Vektor mit dem korreliert wird. Muss nur einmalig erstellt werden.*/
Complex CorrSig[N];
/*Buffer PLL*/
Complex PLLBuffer[N];

Complex *pPLLWorkingBuffer;

float testA[N];
float testB[N];
/*Vektor enthält die detektierten Bits aus der Entscheidung*/
short pBitCompleteMinuteBuffer[59];
/*Index für pBitCompleteMinuteBuffer*/
short minBufind = 0;
/*Aktiviere Bitdekodierung: Dafür auf 1 setzen*/
short decode_string = 0;
/*Ausgabestring für den Text*/
char dateTimeString[100];
/* Sobald startCalc 1 wird, ist ein Bufferpaar voll und es wird begonnen einen
Geschwindigkeitswert zu berechnen. Anschließend wird die Variable wieder auf
0 gesetzt. */
short startCalc = 0;

/* Buffer fuer Speicherung und Darstellung der Korrelationsergebnisse. D.H. Die Abweichungen vom Mittelpunkt*/
short corrErg [V];
/* Index fuer die Verwendung des Korrelationsergebnissebuffers*/
short iCorrErg = 0;
/*Nullen Zählen*/
short count_zero = 0;
/*Nullen Zählen*/
short count_min = 0;
//%Index für die Befüllung des Buffers der PLL. 0
short ind_pll=0;
//%Gibt an das der Input Buffer mit Werten gefüllt wird.
short input_buffer_pll_flag=0;
//%Starten der Berechnung des Phasenrauschens.
short calc_phase_noise = 1;
#ifdef ECHO
/* Speicher zur Ausgabe über die DACs.*/
short inL = 0, inR = 0;
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

#ifdef HARDWARE
/* Sorgt dafür, dass die Variablen im SBSRAM gespeichert werden.*/
#pragma DATA_SECTION(veloc, ".sbsram")
/* Sorgt dafür, dass die Variablen als komplexe Wertepaare gespeichert
werden. Mit stets dem Imaginärteil hinter dem dazugehörigen Realteil.*/
#pragma DATA_ALIGN(w,sizeof(Complex))
#pragma DATA_ALIGN(ABuffer,sizeof(Complex))    
#pragma DATA_ALIGN(BBuffer,sizeof(Complex))  
#endif

//Handelt es sich gerade um das letzte Bit der aktuellen Sequenz und befindet sich der Signalausschnitt in der Absenkung
short flag_absenkung = 0;
/*Speicher für die Zustände des Hilbert Filter*/
float hilbert_zf_i[N_delays_FIR_hilbert/2];
float hilbert_zf_q[N_delays_FIR_hilbert];

/*Init Real Zweig vom HIlbert filter [0 ... 0 1]*/
float hilbert_real[(short)(N_delays_FIR_hilbert / 2) + 1];
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/* Beginn Main()*/
int main(void) {
	

#ifdef SIMU	
	/* Pointer auf Datensätze.*/
	short *pXn = NULL;
	/* Pointer für Datenätze nach Konvertierung nach float.*/
	float *pDataXn = NULL;
	/* Index für das aktuelle Element im Datensatz. */
	unsigned int dataIndex = 0;
	/* Zähler für die Anzahl der Werte im Datensatz. */
	unsigned int lineCountXn = 0;

	/* Erstelle leere Dateien zur Ausgabe, überschreibe alte wenn vorhanden.*/
	FILE * pFile;
	/*"xcorr_indices.txt" enthält die Anzahl der Samples um die die Korrelationen voneinander abweichen.*/
	pFile = fopen("xcorr_indices.txt", "w"); fclose(pFile);

	/* Datensätze aus Datei einlesen */
	lineCountXn = dcf77_readDataFromFile(&pXn, "x_n_s.txt");
	if (pXn == NULL) {
		return EXIT_FAILURE;
	}
	/* Datensätze nach float konvertieren */
	pDataXn = dcf77_sshortToFloat(pXn, lineCountXn);
#endif

#ifdef HARDWARE	
	/* Initialisiere DSP-Board und Codec.*/
	initHardware();
#endif

	/* Initialsierung der Datenbufferpointer.*/
	pInputBuffer = ABuffer;
	pWorkingBuffer = BBuffer;

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

	/*Init Real Zweig vom HIlbert filter [0 ... 0 1]*/
	for(i=0;i<(short)(N_delays_FIR_hilbert/2);i++){
		hilbert_real[i] = 0;
	}
	hilbert_real[i] = 1;

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
		/*Den gesamten Datensatz durchgehen*/
		for (; dataIndex < lineCountXn;) {
			/*Kopiere eingelesene Daten in Buffer. Auf der Hardware	passiert dies im Interrupt.*/
			dcf77_dataAcquisition(&dataIndex, pDataXn, lineCountXn);
#endif

			///////////////////////////////////////////////////////////////
			/* Buffer voll? Starte Berechnungen*/
			if (startCalc == 1) {
#ifdef INCLUDEDATA
				/* Interrupt deaktivieren*/
				CSR &= ~1;
#endif
				puts("StartCalc=1\n");
				/*****Hilbert Filter*******************************/
				//float hilbert_zf_i[N_delays_FIR_hilbert];
				//float hilbert_zf_q[N_delays_FIR_hilbert / 2];
				//float FIR_filter_ANSI_C_float(float FIR_delays[], float FIR_coe[], short N_delays, float x_n)
				//ig_Q, zf_Q] = filter(b_FIR_hilber, 1, sig, zf_Q);

				// ^ (-N / 2) = z^-3
				//sig_I, zf_I] = filter(b_delays, 1, sig, zf_I);
				for (i = 0; i < BUFLEN; i++){
					pWorkingBuffer[i].re = pWorkingBuffer[i].re*AMPLIFIE;
					pWorkingBuffer[i].im = FIR_filter_ANSI_C_float(hilbert_zf_q, hilbert, N_delays_FIR_hilbert + 1, pWorkingBuffer[i].re);
					testA[i] = pWorkingBuffer[i].im;
					pWorkingBuffer[i].re = FIR_filter_ANSI_C_float(hilbert_zf_i, hilbert_real, (short)(N_delays_FIR_hilbert / 2) + 1, pWorkingBuffer[i].re);
					testB[i] = pWorkingBuffer[i].re;
					/**%Hüllkurve bestimmen***************************/
					//  sig = sqrt(sig_I.^2+sig_Q.^2)
					envelope[i] = sqrt(pWorkingBuffer[i].re*pWorkingBuffer[i].re + pWorkingBuffer[i].im*pWorkingBuffer[i].im);
				}
				/**% Schwellen detektieren**********************************/
				//sig_bin = dcf77_bit_decider(sig_bin, sig, 0.8, block_laenge);
				dcf77_bit_decider(envelope, envelope, 0.8, BUFLEN);

				/////////////////////TODO MedianFIlter einbauen!!!
				//for k = 1 : length(sig_bin) - len_median
				for (i = 0; i < BUFLEN - MEDLEN - 1; i++){
					//	sig_bin(k) = dcf77_MedFilt(sig_bin(k:k + len_median), len_median);
/**!!!!!!!!!!!!!!!!!!!!!!!!!!EInfügen der Daten ins MEDIAN FIlter WIE machen in C??***/
///envelope[i] = dcf77_MedFilt(envelope[i:i + MEDLEN], MEDLEN);
					//end
				}
				/***%Hüllkurve abtasten*****************************************/

				//	for i = 1:lenSig
				for (i = 0; i < BUFLEN; i++){
					//if sig_bin(i) == 0
					if (envelope[i] == 0){
						count_zero++;
						count_min = 0;
						//%Der Buffer für die PLL wird bereits mit Beginn der Absenkung befüllt, so
						//	%hat die PLL etwas Zeit sich einzuschwingen.
						//	input_buffer_pll_re(ind_pll) = sig_I(i);
						//input_buffer_pll_im(ind_pll) = sig_Q(i);
						PLLBuffer[ind_pll].re = pWorkingBuffer[i].re;
						PLLBuffer[ind_pll].im = pWorkingBuffer[i].im;
						testA[ind_pll] = PLLBuffer[ind_pll].re;
						testB[ind_pll] = PLLBuffer[ind_pll].im;
						ind_pll++;
						input_buffer_pll_flag = 1;

						//%Handelt es sich gerade um das letzte Bit der aktuellen Sequenz
						if (i == BUFLEN){
							flag_absenkung = 1;
						}
					}
					else
					{
						count_min++;
						//%Hat die Befüllung des PLL Buffers begonnen, so muss diese
						//	%fortgesetzt werden, sobald die ABsenkung vorbei ist und das
						//	%Phasenrauschen beginnt.
						if (input_buffer_pll_flag == 1){
							//input_buffer_pll_re(ind_pll) = sig_I(i);
							//input_buffer_pll_im(ind_pll) = sig_Q(i);
							PLLBuffer[ind_pll].re = pWorkingBuffer[i].re;
							PLLBuffer[ind_pll].im = pWorkingBuffer[i].im;
							testA[ind_pll] = PLLBuffer[ind_pll].re;
							testB[ind_pll] = PLLBuffer[ind_pll].im;
							//ind_pll = ind_pll + 1;
							ind_pll++;
							//end
							//end
						}
					}
					//%Sobald der input_buffer_pll voll ist, muss die Befüllung gestoppt
					//%werden.
					if (ind_pll == N / 2){
						//if ind_pll == buffer_pll_len / 2
						input_buffer_pll_flag = 0;
						//%Working und Inputbuffer wechseln.
						//	temp_re = working_buffer_pll_re;
						//temp_im = working_buffer_pll_im;
						//working_buffer_pll_re = input_buffer_pll_re;
						//working_buffer_pll_im = input_buffer_pll_im;
						//input_buffer_pll_re = temp_re;
						//input_buffer_pll_im = temp_im;
						//%Befüllen des neuen InputBuffers vorbereiten.
						ind_pll = 1;
						//%Starten der Berechnung des Phasenrauschens.
						calc_phase_noise = 1;
						//end
						//	end*/
					}
				}

				/**%Neue Minute Markierung erkennen*********************/
				/*%Neue Minute Markierung erkennen
					% Fs = 5438, 6Hz = > 5438 Samples pro Sekunde.Wenn für >5438 Samples keine
					% Nullen auftauchen, somuss es sich um den Minutenübergang handeln. + 200 ist
					% Sicherheitsabstand.*/
				//if count_min > round(Fs) + 200
				if (count_min > (short)FSAMP + 200){
					//	disp(length(erg));
					//if (strcmp(file, 'simu'))
					if (decode_string){
						dcf77_bitdecoder(dateTimeString);
					}
					//	disp(dcf77_bitdecoder(erg));  %TR
					//	end
					//	erg = [];
					//disp('NEUE MINUTE-----------------------');
					//end
				}


				/**%Länge der Absenkung detektieren -> Bits detektieren*****************************************************/
				/*
				% 564 entspricht, 600 schafft etwas Toleranz
				% 100ms, Logisch 0
				% 200ms, Logisch 1*/
				//if count_zero < 600 && count_zero > 50 && flag_absenkung == 0
				if (count_zero < 600 && count_zero > 50 && flag_absenkung == 0){
					//	disp('Null entdeckt');
					//erg = [erg(:)',0];
					pBitCompleteMinuteBuffer[minBufind] = 0;
					minBufind++;
					//	%Bit erkannt, wieder auf Null setzen
					count_zero = 0;
					//elseif(count_zero > 600 && flag_absenkung == 0) % 200ms
				}
				else if (count_zero > 600 && flag_absenkung == 0){
					//	disp('-- Eins entdeckt');
					//erg = [erg(:)',1];
					pBitCompleteMinuteBuffer[minBufind] = 1;
					minBufind++;
					//	%Bit erkannt, wieder auf Null setzen
					count_zero = 0;
					//end
				}

				/**% Phasenrauschen bestimmen**********************************/
				/** % Maximum der KKF bestimmen.*****************************************************/
				/*******************************************************/
				/*******************************************************/
				/////////////////////////////////////////////////////////////






#ifdef INCLUDEDATA						
				/* Vergleiche Korrelationsergebnisse.*/
				compCorr();
				send_string("Vergleich Korrelation abgeschlossen. Programm endet. \r\n");
				return EXIT_SUCCESS;
#endif

#ifdef SIMU				
				//				writeCompReToFile(pWorkingBuffer, N - 1, "corr1_proc.txt");
#endif


#ifdef HARDWARE
				/*Gebe den aktuellen Geschwindigkeitswert über das Terminal aus.*/
				/*Umbennen veloc[iVelo]  */
				send_string(stoa((short)(veloc[iVelo - 1] * SCALETERMINAL)));
#endif
				/*
				#ifdef SIMU
				//////////Umbennen veloc[iVelo]
				writeDataToFile(&(veloc[iVelo - 1]), 1, "v_proc.txt");
				#endif
				*/
				//Berechnungen abgeschlossen
				startCalc = 0;
			}

		}
#ifdef SIMU
			/*Berechnung abgeschlossen. Gebe allokierten Speicher frei.*/
			free(pDataXn); pDataXn = NULL;
			puts("!!!Berechnung abgeschlossen!!!");
			system("pause");
			return EXIT_SUCCESS;
#endif
		
	}



