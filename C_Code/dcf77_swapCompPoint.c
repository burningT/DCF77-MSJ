/* swapCompPoint.c 

Vertauscht die beiden übergebenen Pointer vom Typ Complex. *
Parameter:
Complex ** a: Call by Reference, Pointer 1
Complex ** b: Call by Reference, Pointer 2
*/ 

#include "dcf77_includes.h"
void dcf77_swapCompPoint(Complex ** a, Complex ** b){
	Complex * temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
