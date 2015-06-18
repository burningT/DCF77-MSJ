#include "Testsuite.h"


void dcf77_vcoTest(void){

	float vi_out;
	float vq_out;
	float y = 2;
	float ts = 5438;
	float wc = 5438/4;
	float kv = 50;
	float z = 0;

	//Start test
	printf("---dcf77_vcoTest---\n");

	dcf77_vco(&vi_out, &vq_out, y, ts, wc, kv, &z);

	//printf("Erwarteter Wert: %f\n", expResult);

	printf("Erhaltener Wert I: %f, Q:%f\n", vi_out, vq_out);
}
