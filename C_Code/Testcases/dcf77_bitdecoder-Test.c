#include "Testsuite.h"

void dcf77_bitdecoderTest(void){
	/*short testArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
						0, 0, 0, 0, 1, 1, 0, 1, 1, 1,
						1, 0, 0, 0, 1, 1, 0, 0, 0, 0,
						0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
						0, 0, 1, 0, 1, 0, 0, 0, 0 };*/

	char expString[] = "23:58 Uhr, Dienstag 20.05.2014";

	char dateTimeString[DATE_TIME_STRING_LEN];

	//Start test
	printf("---dcf77_bitdecoderTest---\n");
	
	dcf77_bitdecoder( dateTimeString);

	printf("Erwarteter String: %s\n", expString);
	printf("Erhaltener String: %s\n", dateTimeString);

	return;
}

