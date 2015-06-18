/* initHardware.c
 * Initialisiert und konfiguriert die Hardware. Es werden Information �ber
 den aktiven Betriebsmodus �ber das Terminal ausgegeben.
 */
 
#include "includes.h"

#ifdef HARDWARE
void initHardware(){

	/* Initialisierung*/
	init_module(0);
	/* Resete ADDA16 board */
	clr_module_cfg(CFG_nRESOUT);
	delay(10);
	set_module_cfg(CFG_nRESOUT);
	/*	Ausgabe �ber Terminal */
	send_string(" --------------------------------------------------- \r\n");
	send_string("|         Beruehrungslose Geschwindigkeitsmessung   |\r\n");
	send_string("| 28.05.2014                                        |\r\n");
	send_string("| Aktivierte Praeprozessor Konstanten:              |\r\n");
#ifdef INCLUDEDATA
	send_string("| INCLUDEDATA 	Daten aus .h-Dateien                |\r\n");
#endif
#ifdef REALTIME
	send_string("| REALTIME 	Daten von den ADCs                  |\r\n");
#endif
#ifdef SHOWCORR
	send_string("| SHOWCORR 	Betrachten der letzten xcorr        |\r\n");
#endif
#ifdef ECHO
	send_string("| ECHO 		ADCs direkt nach DACs               |\r\n");
#endif
	send_string(" --------------------------------------------------- \r\n\n");
	
	/* Binde adda16_adcint() als interrupt handler ein.*/
	install_interrupt(CPU_INT4, adda16_adcint);
	
	/* Konfiguriere und starte ADDA16 board. Es wird ein externes Taktsignal
	verwendet. Konfiguriere LDAC f�r synchrones updaten des DAC mit dem
	aufnehmen von Werten �ber den ADC. Gebe das Taktsignal �ber den Ausgang
	EXTCLK_OUT aus.*/
	ADDA16 ->fs = ADDA16_FSEXT;
	ADDA16 ->cfg = (ADDA16_EXTCLK_ON | ADDA16_INT0ADC | ADDA16_LDAC_ANY); 
	return ;
}
#endif
