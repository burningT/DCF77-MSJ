/** dcf77_phase_detector.c (TR 17-June-2015)
*
* Edited by:
*
*                  Calculates the phase difference between the Complex Signal 'Inp' and 'VCO'
*/
#include "dcf77_includes.h"

float dcf77_phase_detector(float Inp_I, float Inp_Q, float VCO_I, float VCO_Q){	//TODO: welcher Datentyp wird erwartet?

	float tmp_I;
	float tmp_Q;

	// Complex Multiplication : z1 * z2 = (a1*a2+a1*jb2+jb1*a2-b1*b2)    
	tmp_I = Inp_I * VCO_I - Inp_Q * VCO_Q;
	tmp_Q = Inp_I * VCO_Q + Inp_Q * VCO_I;

	return atan2(tmp_Q, tmp_I);
}