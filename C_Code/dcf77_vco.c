/** dcf77_vco.c (TR 17-June-2015)
*
* Edited by:
*
*                  Voltage Controlled Oscillator
*
*                   This function generates a complex sinus signal based on
*                   its input y. The generation of the signal is based on
*                   the functionality of a common VCO as part of an PLL.
*                   
*
*       y           : Input signal (filtered result of Phase Detector)
*       ts          : Sampling period
*       wc          : Reference frequency of the VCO
*       kv          : Amplification of the input signal
*       *z          : delay
*       vi_out      : In phase output of the VCO
*       vq_out      : Quadrature output of the VCO
*
*/
#include "dcf77_includes.h"

void dcf77_vco(float *vi_out, float *vq_out, float y, float ts, float wc, float kv, float *z){	//TODO: welcher Datentyp wird erwartet?

	//Amplify input
	y = y*kv*ts;

	//Regulate output - frequency
	*z = *z + y + wc*ts;
	y = *z;

	//prevent overflow
	if (y > PI)
		y = y - 2 * PI;
	else if (y < -PI)
		y = y + 2 * PI;

	//Generate output signal
	*vi_out = cos(-y);
	*vq_out = sin(-y);

	return;
}