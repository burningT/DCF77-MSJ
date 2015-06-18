/***********************************************************************

  adda16.cmd - D.Module.C671x Linker Command File

  Revision  1.0
  Company   D.SignT
  Author    Adolf Klemenz
  Created   12 Mar 2003

  Hardware  D.Module.C6713
  Compiler
  Options

  History   1.0 initial release  12 Mar 2003 by AK

***********************************************************************/

/***********************************************************************
  Code Composer: remove the following entries and use the Code Composer
  Project Build Options Dialog instead
***********************************************************************/
-c
-x
-w
-stack 0x1000
-heap 0x400
-l rts6700.lib

/***********************************************************************
  MEMORY defines the available physical memory areas
***********************************************************************/
MEMORY
{
    VECT    : org = 0x00000000, len = 0x00000200

    /*
       the following memory areas are reserved for the BIOS functions
       BIOSTEXT: org = 0x00000200, len = 0x00000F00
       BIOSDATA: org = 0x00001100, len = 0x00000100
    */

    /*
       the following memory areas are reserved for the CONFIG
       utility functions
       CFGTEXT : org = 0x00001200, len = 0x00001F00
       CFGDATA : org = 0x00003100, len = 0x00000400

       use this IRAM mapping if you want to use the Config Utility
       functions in your program

    IRAM    : org = 0x00003500, len = 0x0002CB00

       use this IRAM mapping if you do NOT use the Config Utility
       functions in your program
    */
    IRAM    : org = 0x00001200, len = 0x0002EE00


    SBSRAM  : org = 0x80000000, len = 0x00080000
    SDRAM   : org = 0xB0000000, len = 0x01000000
}

/***********************************************************************
  SECTIONS defines the mapping of compiler sections to physical memory
***********************************************************************/
SECTIONS
{
    .vect     > VECT        /* only required if you link your own IVT */
    .text     > IRAM
    .cinit    > IRAM /*SBSRAM*/ /* must be in IRAM. reason to be investigatred !! US 18-Aug-06*/
    .stack    > IRAM
    .data     > IRAM        /* .data is not used by the C compile     */
    .far      > IRAM //IRAM/*auf SBSRAM aendern, wenn die Performance dies erlaubt um N=2048 zu ermoeglichen. zum test von TB geaendert*/
    .bss      > IRAM
    .const    > IRAM 
    .switch   > IRAM
    .sysmem   > SBSRAM
    .sbsram   > SBSRAM
    .cio      > SBSRAM
}



