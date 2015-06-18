/** dcf77_loop_filter.c (TR 17-June-2015)
*
* Edited by:
*
*                  Loop filter for the usage in the PLL
*                  
*
*       y           : Input signal (result of Phase Detector)
*       *z[2]       : delays states initial/final 
*       out         : Filtered output signal
*       alpha       : 
*       beta        :
*
*/
#include "dcf77_includes.h"

float dcf77_loop_filter(float y, float *z, float alpha, float beta){	//TODO: welcher Datentyp wird erwartet?

	float out = 0;
	float up;
	float down;

	float coeffs[] = {beta, 1, 1};

	up = IIR_DF1(coeffs, z, y);

	//[up, delay] = filter([beta], [1 - 1], y(k), z*);
	down = y * alpha;
	out = up + down;

	return out;
}