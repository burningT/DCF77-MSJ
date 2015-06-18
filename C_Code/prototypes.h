/* prototypes.h
Die gesammelten Prototypen der verwendeten Funktionen.
 */
 
#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_

#include "typedefs.h"

void calcCorr(short iFFT []); 
float calcVelo(); 
void initAdapFilt( adapFilt * filt, short CH);
void applyFilter(adapFilt * af1, adapFilt * af2);
void NLMS(adapFilt * af, Complex * d);
void dotProduct(float * res, float * x, float * y, short count);
void swapCompPoint(Complex ** a, Complex ** b);
void sortBub(float arr[], short n);
float medFilt(float * input);

void dcf77_bitdecoder(int rawArray[], char dateTimeString[]);

/*Optimierte Funktionen von Texas Instruments*/
void cfftr2_dit(Complex* x, Complex* w, short n);
void bitrev(Complex *xs, short *index, int n);
void digitrev_index(short *index, int n, int radix);

#ifdef SIMU
unsigned int readDataFromFile(short ** pCH, char fileName []);
void writeDataToFile(float * var, unsigned int numOfElements, char * fileName);
void writeComeReToFile(Complex * var, int numOfElements, char * fileName);
void dataAcquisition(unsigned int * dataIndex, float * xn, float * yn, unsigned int lineCount);
float * sshortToFloat(short * pIn, unsigned int count);
#endif

#ifdef HARDWARE
interrupt void adda16_adcint(void);
void initHardware();
/* Funktionen zur Verfügung gestellt von DSIGNT*/
void send_string(char *s);
char *stoa(short x);
#endif

#ifdef INCLUDEDATA
void compCorr();
#endif

#endif /* PROTOTYPES_H_ */
