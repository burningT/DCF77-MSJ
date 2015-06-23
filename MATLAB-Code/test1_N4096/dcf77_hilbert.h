//------------------------------------------- 
// designed with -- dcf77_main.m -- 
// Fs = 5438.60
// fstop = 2175.44
// fpass = 3263.16
// N_FIR = 6
//------------------------------------------- 
#define N_delays_FIR_hilbert 6

// DEC FILTER
float H_filt_FIR_hilbert[N_delays_FIR_hilbert]; 
float hilbert[7]={
 -0.067335, 0.000000, -0.567117, 0.000000, 0.567117, -0.000000,
 0.067335,};

