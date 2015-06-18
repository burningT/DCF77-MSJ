#include "Testsuite.h"

Complex BPLLBuffer[N];

Complex *pPLLWorkingBuffer = BPLLBuffer;



short BitCompleteMinuteBuffer[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
									0, 0, 0, 0, 1, 1, 0, 1, 1, 1,
									1, 0, 0, 0, 1, 1, 0, 0, 0, 0,
									0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
									0, 0, 1, 0, 1, 0, 0, 0, 0 };

short *pBitCompleteMinuteBuffer = BitCompleteMinuteBuffer;

main(void){

	dcf77_bitdecoderTest();
	dcf77_bitdeciderTest();
	dcf77_phase_detectorTest();
	dcf77_vcoTest();
	dcf77_loop_filterTest();
}