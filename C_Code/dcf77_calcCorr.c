/* calcCorr.c
Berechnet die Sch�tzung der Kreuzkorrelation der Signale beider Kan�le.

Parameter:
short iFFT []: Indices f�r die Neuanordnung der inversen FFT.
*/

#include "dcf77_includes.h"

extern Complex * pWorkingBuffer;

extern Complex w[N / RADIX];

extern Complex CorrSig[N];

void calcCorr(short iFFT []){
	int i=0;
	/*Pointer auf corrSig*/
	Complex * pCorrSig = CorrSig;
	float reTemp =0.0f;	
	/*Fuehre Zero-Padding durch.
	Beispiel fuer BUFLEN = 4, a1 bis a4 sind die Werte aus dem ADC
	Index 	0        BUFLEN-1       N-1
	Inhalt [a1,a2,ar,a4      ,0,0,0,0  ]
	*/
	for(i=BUFLEN;i<N;i++){
		pWorkingBuffer[i].re = 0.0f;
		pWorkingBuffer[i].im = 0.0f;
		//pWorkingBufferCH2[i].re = 0.0f;
		//pWorkingBufferCH2[i].im = 0.0f;
	}
#ifdef SIMU
	for(i=0;i<50;i++) printf("Sig xn: i=%d \t re= %10.4f im= %10.4f\n",i,pWorkingBuffer[i].re,pWorkingBuffer[i].im);
#endif
	/* F�hre FFT aus und ordne das Ergebnis neu an.*/
	cfftr2_dit(pWorkingBuffer, w , N);
	cfftr2_dit(pCorrSig, w, N);
	bitrev(pWorkingBuffer, iFFT, N );
	bitrev(pCorrSig, iFFT, N );
	
#ifdef SIMU
	for(i=0;i<50;i++) printf("FFT res xn: i=%d \t re= %10.4f im= %10.4f\n",i,pWorkingBuffer[i].re,pWorkingBuffer[i].im);
#endif

	/* F�hre Multiplikation im Frequenzbereich aus. Das Ergebnis wird in Kanal 1 gespeichert*/
	for(i=0;i<N;i++){
		/* Der Realteil muss zwischengespeichert werden, da der Realteil �berschrieben wird,
		aber noch f�r den Imagin�rteil ben�tigt wird.*/
		reTemp = pWorkingBuffer[i].re;
		pWorkingBuffer[i].re = pWorkingBuffer[i].re * pCorrSig[i].re - pWorkingBuffer[i].im * pCorrSig[i].im;
		/* Das negative Vorzeichen erlaubt die IFFT mit den selben Twiddlekonstanten durchzuf�hren.*/
		pWorkingBuffer[i].im = -(reTemp * pCorrSig[i].im + pWorkingBuffer[i].im * pCorrSig[i].re);
	}	 
	/* F�hre die inverse FFT aus. Findet inplace im Buffer statt*/
	cfftr2_dit(pWorkingBuffer, w , N);
	bitrev(pWorkingBuffer, iFFT, N );
	
	/* Skaliere jedes Element mit der Anzahl der Elemente entsprechend der Berechnungsvorschrift.
	Das letzte sample muss entfernt werden um ein  Korrelationsergebnis mit 2*BUFLEN-1 Werten zu
	erhalten. Dies geschieht indem das letzte Sample im n�chsten Verarbeitungsschritt nicht
	verwendet wird. Aus diesem Grund braucht die Schleife nur bis N-1 zu laufen. Geteilt durch
	BUFLEN um die 'biased' Eigenschaft aus Matlab zu erhalten.*/
	for(i=0;i<N-1;i++){
		pWorkingBuffer[i].re = pWorkingBuffer[i].re / (float)N / (float)BUFLEN ;
		
#ifdef SHOWCORR
		/* Die Variable rxy kann in CCS geplottet werden. So kann das Ergebnis der Korrelation betrachtet werden.*/
		rxy[i] = pWorkingBuffer[i].re;
#endif

	}
	
#ifdef SIMU
	for(i=0;i<50;i++) printf("IFFT : i=%d \t re= %10.6f im= %10.6f\n",i,pWorkingBuffer[i].re,pWorkingBuffer[i].im);
#endif
	return;
}