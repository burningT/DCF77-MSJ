#include "Testsuite.h"


void dcf77_phase_detectorTest(void){

	float c1_i = 1.0;
	float c1_q = 0;
	float c2_i = 0;
	float c2_q = 1.0;
	
	float expResult = PI/2;
	float resultingResult;

	//Start test
	printf("---dcf77_phase_detectorTest---\n");

	resultingResult = dcf77_phase_detector(c1_i, c1_q, c2_i, c2_q);

	printf("Erwarteter Wert: %f\n", expResult);

	printf("Erhaltener Wert: %f\n", resultingResult);
}