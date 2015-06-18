#include "Testsuite.h"


void dcf77_bitdeciderTest(void){
	int i;

	float testArray[] = { 12, 15.0, 18.1, 19, 14, 12, 10 };
	int len_testArray = 7;
	float threshold = 15.0;

	float expArray[] = { 0, 1, 1, 1, 0, 0, 0 };

	float resultArray[7];
	
	//Start test
	printf("---dcf77_bitdeciderTest---\n");

	dcf77_bit_decider(resultArray, testArray, threshold, len_testArray);

	printf("Erwartetes Array: ");
	
	for (i = 0; i < len_testArray; i++)
		printf("%.1f ", expArray[i]);
	
	printf("\n");
	printf("Erhaltener Array: ");
	
	for (i = 0; i < len_testArray; i++)
		printf("%.1f ", resultArray[i]);
	
	printf("\n");

	return;
}

