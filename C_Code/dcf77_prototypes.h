/* dcf77_prototypes.h
Die gesammelten Prototypen für die DCF77-Files der verwendeten Funktionen.
*/

#ifndef DCF77_PROTOTYPES_H_
#define DCF77_PROTOTYPES_H_

void dcf77_bitdecoder(char dateTimeString[]);
void dcf77_bit_decider(float sig_bin[], float sig[], float threshold, int lenSig);
float dcf77_phase_detector(float Inp_I, float Inp_Q, float VCO_I, float VCO_Q);
void dcf77_vco(float *vi_out, float *vq_out, float y, float ts, float wc, float kv, float *z);
float dcf77_loop_filter(float y, float *z, float alpha, float beta);
int dcf77_pll(float fs);

void dcf77_swapCompPoint(Complex ** a, Complex ** b);
float dcf77_medFilt(float * input);
void dcf77_sortBub(float arr[], short n);

/*Optimierte Funktionen von Texas Instruments*/
void cfftr2_dit(Complex* x, Complex* w, short n);
void bitrev(Complex *xs, short *index, int n);
void digitrev_index(short *index, int n, int radix);

float IIR_DF1(float *num_envelope, float *T_IIR, float x_n);

#ifdef SIMU
unsigned int dcf77_readDataFromFile(short ** pCH, char fileName[]);
void dcf77_writeDataToFile(short * var, unsigned int numOfElements, char * fileName);
//void writeComeReToFile(Complex * var, int numOfElements, char * fileName);

void dcf77_dataAcquisition(unsigned int * dataIndex, float * xn, unsigned int lineCount);
float * dcf77_sshortToFloat(short * pIn, unsigned int count);

float FIR_filter_ANSI_C_float(float FIR_delays[], float FIR_coe[], short N_delays, float x_n);

#endif

#ifdef TI_COMPILER_USED
interrupt void adda16_adcint(void);
void initHardware();
/* Funktionen zur Verfügung gestellt von DSIGNT*/
void send_string(char *s);
char *stoa(short x);
#endif


#endif /* DCF77_PROTOTYPES_H_ */
