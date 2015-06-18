/***********************************************************************

    adda16.h - D.Module.ADDA16 Header File

    Revision    1.0
    Company     D.SignT
    Author      Adolf Klemenz
    Created     18 Nov 2004

    Hardware    D.Module.ADDA16 Rev.1
                D.Module.C6713
    Compiler
    Options

    History     1.0 initial release  18 Nov 2004 by AK

***********************************************************************/
#ifndef __ADDA16__
#define __ADDA16__

/***********************************************************************

    ADDA16 Base Address

    Addrese Decoding Scheme (-: jumper open, x: jumper closed):

     JPA5 | JPA4 | ADDA16_OFFSET
    ------+------+---------------
       -  |   -  |   0x000000
       -  |   x  |   0x000040
       x  |   -  |   0x000080
       x  |   x  |   0x0000C0
      
    JPA18 | JPA17 | JPA16 | ADDA16_OFFSET
    ------+-------+-------+---------------
      x   |   -   |   -   |   0x000000
      x   |   -   |   x   |   0x040000
      x   |   x   |   -   |   0x080000
      x   |   x   |   x   |   0x0C0000

    
***********************************************************************/
#define ADDA16_OFFSET   0   /* default value */

#define ADDA16_BASE     (IOSEL_BASE + ADDA16_OFFSET)

typedef struct
{
    volatile short rsvd0;
    volatile short ch0;
    volatile short rsvd1;
    volatile short ch1;
    volatile short rsvd2;
    volatile short ch2;
    volatile short rsvd3;
    volatile short ch3;
    volatile short rsvd4;
    volatile unsigned short fs;
    volatile short rsvd5;
    volatile unsigned short cfg;
} adda16_type;

#define ADDA16 ((adda16_type *) ((unsigned int *)ADDA16_BASE))

/***********************************************************************

  Macros to read ADC and write DAC:

    channel_0 = ADDA16_READ_ADC(0); // read ADC0 and sign-extend and shift data to D15..D0
    ADDA16_WRITE_DAC(2, 0x3FFF);    // write 0x3FFF to DAC2

***********************************************************************/
#define ADDA16_READ_ADC(ch)       (*(volatile short*)(ADDA16_BASE+2+(ch<<2)))
#define ADDA16_WRITE_DAC(ch,val)  (*(volatile short*)(ADDA16_BASE+2+(ch<<2)) = val)


/***********************************************************************

  ADDA16 Sampling Frequency Register

***********************************************************************/
/* common sampling frequencies */
#define ADDA16_FS250    0x000F
#define ADDA16_FS200    0x0013
#define ADDA16_FS160    0x0018
#define ADDA16_FS120    0x001F
#define ADDA16_FS100    0x0027
#define ADDA16_FS80     0x0031
#define ADDA16_FS67     0x003B
#define ADDA16_FS50     0x004F
#define ADDA16_FS33     0x0077
#define ADDA16_FS25     0x009F
#define ADDA16_FS20     0x00C7
#define ADDA16_FS16     0x00F9
#define ADDA16_FSEXT    0x0000

/* this macro can be used for any integer sampling frequency from 15625 Hz to 250 kHz */
#define ADDA16_FS(x)    ((4000000/(int)x)-1)

/***********************************************************************

  ADDA16 Configuration Register

***********************************************************************/
#define ADDA16_DAC_READY 0x0080  /* read : DAC ready for new transfer */
#define ADDA16_EXTCLK_ON 0x0080  /* write: turn on EXTCLK output */

#define ADDA16_INT1DAC   0x0040  /* map DAC to nINT1 */
#define ADDA16_INT1ADC   0x0020  /* map ADC to nINT1 */
#define ADDA16_INT1FS    0x0060  /* map sampling frequency to nINT1 */
#define ADDA16_INT0DAC   0x0010  /* map DAC to nINT0 */
#define ADDA16_INT0ADC   0x0008  /* map ADC to nINT0 */
#define ADDA16_INT0FS    0x0018  /* map sampling frequency to nINT0 */

#define ADDA16_LDAC_ANY  0x0000  /* update DACs after any DAC write */
#define ADDA16_LDAC_2    0x0001  /* update DACs after writing to DAC 2 */
#define ADDA16_LDAC_3    0x0002  /* update DACs after writing to DAC 3 */
#define ADDA16_LDAC_4    0x0003  /* update DACs after writing to DAC 4 */
#define ADDA16_LDAC_FS   0x0004  /* use ADC sampling frequency to update DACs */



#endif  /* __ADDA16__ */
    
