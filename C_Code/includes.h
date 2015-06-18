/* includes.h
Enthält die gesammelten #include Anweisungen. So muss in den .c-Dateien
 immer nur dieses eine Header eingebunden werden.
 */
 
#ifndef INCLUDES_H_
#define INCLUDES_H_

/* Symbolische Konstanten*/
#include "defines.h"
/* Prototypen für Funktionen*/
#include "prototypes.h"
#include <math.h>
#include <stdlib.h>
/*Eigene Datentypen*/
#include "typedefs.h"

#ifdef SIMU
#include <stdio.h>
#endif

#ifdef HARDWARE
#include "bios.h"
#include "adda16.h"
#endif

#endif /* INCLUDES_H_ */
