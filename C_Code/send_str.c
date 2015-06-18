//#include "bios.h"
//#include "adda16.h"
#include "includes.h"



/*#include "bios.h"
#include "adda16.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
*/
/***********************************************************************

    function    char * stoa (short x)

    convert a short integer into a base-10 ASCII string

    Parameter   x - short value to convert
    Returns     pointer to string
    Calls       -

    This function uses floating point multiplication instead of
    integer division and multiplication to prevent interrupt
    blocking: the integer div implementation of the default runtime
    library is compiled without the -mi switch and may block
    interrupts for quite a long time.

***********************************************************************/
char *stoa (short x)
{
    /*******************************************************************
      locals
    *******************************************************************/
	static char s[9];
	char *p = s;
	int i;
	short temp;

    float fmul_1 = 0.0001;
    float fmul_2 = 10000.0;
	
    /*******************************************************************
      calculate sign
    *******************************************************************/
	if (x < 0) {
		*p = '-';
		x=-x;
	}else 
		*p = ' ';
	p++;
	
    /*******************************************************************
      calculate 5 digits
    *******************************************************************/
	
	for (i=0; i<5; i++)
	{
        temp = (short)((float)x * fmul_1);
		*p++ = temp + '0';
        x -= (short)((float)temp * fmul_2);
        fmul_2 = fmul_2 * 0.1;
        fmul_1 = fmul_1 * 10.0;
	}
	*p++='\r';
	*p++='\n';
	*p = 0;

	return (s);	
}

/***********************************************************************

    function    void send_string (char *s)

    transmit a 0-terminated string via the UART
    this function makes full use of the UART's transmit FIFOs
    it can also be used to transmit strings located in Flash Memory,
      because the characters are ANDed with FF to mask any undefined
      bits when read from non-32Bit wide memory

    Parameter   *s Pointer to string
    Returns     -
    Calls       BIOS function send_char

***********************************************************************/
void send_string (char *s)
{
    char *sp = s;
	//char *sp = "test";
    int i;

    while ( *sp & 0xFF )
    {
        /***************************************************************
          wait until transmit hold register = transmit FIFO empty
        ***************************************************************/
        while ( ! send_char(*sp) );

        /***************************************************************
          we can now store up to 31 additional characters in the
          transmit FIFO
        ***************************************************************/
        i = 0;
        while ( (*++sp & 0xFF) && (++i < 31) ) UART->UART_THR = *sp;
    }
	return;
}
