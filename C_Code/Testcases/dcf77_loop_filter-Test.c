#include "Testsuite.h"

void dcf77_loop_filterTest(void){

	float y = 1.5;
	float z[] = { 0, 0 };
	float alpha = 1;
	float beta = 1;

	float resultingResult;

	//Start test
	printf("---dcf77_loop_filterTest---\n");

	resultingResult = dcf77_loop_filter(y, z, alpha, beta);

	//printf("Erwarteter Wert: %f\n", expResult);

	printf("Erhaltener Wert: %f\n", resultingResult);
}