/** dcf77_pll.c (TR 17-June-2015)
*
* Edited by:
*
*                  PLL for calculation of phaseshifts in a signal
*                   
*					Die Funktion erwartet das globales Array Complex *pPLLWorkingBuffer der Länge N
*					Die Ergebnisse der PLL werden unter pPLLWorkingBuffer[x].re gesichert
*
*
*       fs				: Sampling frequency
*
*/
#include "dcf77_includes.h"
extern Complex *pPLLWorkingBuffer;

int dcf77_pll(float fs){

	//general variables
	int i;
	float pdOut;

	//Initial values
	float vco_i = 0;
	float vco_q = 0;
	float z_vco = 0;
	float z_lf[] = { 0, 0 };

	//Parameters
	float lf_alpha = 1;
	float lf_beta = 1 / 100;
	float vco_kv = 50;
	float vco_wc = 2 * PI*fs / 4;
	
	for (i = 0; i < N; i++){
		pdOut = dcf77_phase_detector(pPLLWorkingBuffer[i].re, pPLLWorkingBuffer[i].im, vco_i, vco_q);
		pPLLWorkingBuffer[i].re = dcf77_loop_filter(pdOut, z_lf, lf_alpha, lf_beta);
		dcf77_vco(&vco_i, &vco_q, pPLLWorkingBuffer[i].re, 1 / fs, vco_wc, vco_kv, &z_vco);
	}
}