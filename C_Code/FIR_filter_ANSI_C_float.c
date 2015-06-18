//------------------------------------------------------
// File: FIR_filter_ANSI_C_float.c
// =====
//
// US 27-Dec-11
// 
// Description:
// ============
// programm to do FIR filter computation in a subroutine
//
// Note : 
// The number of delays and coeffs are the same.
// Therefore, FIR_delays[0] will contain x(n).
// 
// Changes:
//---------------------------------------------------------------------
float FIR_filter_ANSI_C_float(float FIR_delays[], float FIR_coe[], short N_delays, float x_n)
{
		short i;
		float FIR_accu32=0;
//---------------------------------------------------------------------
// delays BACKWARDS, coefficients in FORWARD direction
		FIR_delays[N_delays-1] = x_n;	// read input sample from ADC 
// accumulate in 32 bit variable
		FIR_accu32	= 0;				// clear accu
		for(i=0; i < N_delays; i++)		// FIR filter routine
			FIR_accu32 += FIR_delays[N_delays-1-i] * FIR_coe[i];
	
// loop to shift the delays
		for(i=1; i < N_delays; i++)				
			FIR_delays[i-1] = FIR_delays[i];

		return FIR_accu32;
}

