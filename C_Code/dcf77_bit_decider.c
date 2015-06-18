/** dcf77_bit_decider.c (TR 17-June-2015)
*
* Edited by: 
*
*                  Function for binary decision.
*
*
*		sig_bin         : Buffer for the Signal after the decision.//TODO:pr�fen, ob etwas �berschrieben wird!!
*                           Use thereference to avoid additional
*                           memory allocation
*       sig             : The input signal, shall have the same length as
*                           sig_bin
*       threshold       : Threshhold for the decision
*       lenSig          : Length of sig and sig_bin
*/

#include "dcf77_includes.h"

void dcf77_bit_decider(float sig_bin[], float sig[], float threshold, int lenSig){	//TODO: welcher Datentyp wird erwartet?

	//sig_bin = (short[LEN]){0};		//Initialisierung, damit sig_bin nicht ungewollt �berschrieben werden kann
	int i;									//Schleifenz�hler

	for (i = 0; i < lenSig; i++){
		if (sig[i] < threshold)
			sig_bin[i] = 0;
		else
			sig_bin[i] = 1;
	}
	return;
}
