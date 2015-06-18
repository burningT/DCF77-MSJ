/* dcf77_includes.h
Enthält die gesammelten #include für die DCF77-Files Anweisungen. So muss in den .c-Dateien
immer nur dieses eine Header eingebunden werden.
*/

#ifndef DCF77_INCLUDES_H_
#define DCF77_INCLUDES_H_

#define SIMU 1
//#define TI_COMPILER_USED 1

/* Symbolische Konstanten*/
#include "dcf77_defines.h"
/*Eigene Datentypen*/
#include "dcf77_typedefs.h"
/* Prototypen für Funktionen*/
#include "dcf77_prototypes.h"
/*EXIT_SUCCESS...*/
#include <stdlib.h>
/*Sin,cos...*/
#include <math.h>



#ifdef TI_COMPILER_USED
#include "bios.h"
#include "adda16.h"

#endif /*TI_COMPILER_USED*/

#include <stdio.h>


#endif /* DCF77_INCLUDES_H_ */
