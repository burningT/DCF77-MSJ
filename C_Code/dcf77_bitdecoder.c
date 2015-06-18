/** dcf77_bitdecoder.c (TR 10-June-2015)
*
* Edited by: 
*
*                  Decoder für die Bitfolge des DCF77-Signals
*                    Dekodiert die Bitfolge einer Minute des DCF77-Signals
*					 Bitfolge einer Minute (59 Werte erwartet) in globaler Variablershort *pBitCompleteMinuteBuffer erwartet
*
*		dateTimeString[]: String, in den die Ausgabe gelegt wird (mindestens Länge 50)
*/

#include "dcf77_includes.h"
	extern short minBufind;//TB 17.06.15 eingebaut
	extern short pBitCompleteMinuteBuffer[59];

void dcf77_bitdecoder(char dateTimeString[]){	//TODO: welcher Datentyp wird erwartet?


	//Parameters
	const int year_offset = 2000;

	//Fields to be interpret
	short bit_R  = -1;
	short bit_A1 = -1;
	short bit_Z1 = -1;
	short bit_Z2 = -1;
	short bit_A2 = -1;
	short bit_S  = -1;

	short val_min = -1;
	short sum_min = -1;
	short bit_P1  = -1;

	short val_h  = -1;
	short sum_h  = -1;
	short bit_P2 = -1;

	short val_d = -1;
	short sum_d = -1;

	short val_wd = -1;
	short sum_wd = -1;

	short val_m = -1;
	short sum_m = -1;

	short val_y  = -1;
	short sum_y  = -1;
	short bit_P3 = -1;

	//Checksums
	short checksum_P1 = -1;
	short checksum_P2 = -1;
	short checksum_P3 = -1;

	//Conversion Buffers
	char stringBuf_a2i[7] = { 0 };
	char stringBuf_out[DATE_TIME_STRING_LEN] = { 0 };


	//Organize bits
	bit_R  = pBitCompleteMinuteBuffer[15];	//Bit 15
	bit_A1 = pBitCompleteMinuteBuffer[16];	//Bit 16
	bit_Z1 = pBitCompleteMinuteBuffer[17];	//Bit 17
	bit_Z2 = pBitCompleteMinuteBuffer[18];	//Bit 18
	bit_A2 = pBitCompleteMinuteBuffer[19];	//Bit 19
	bit_S  = pBitCompleteMinuteBuffer[20];	//Bit 20

	val_min = pBitCompleteMinuteBuffer[21] + 2 * pBitCompleteMinuteBuffer[22] + 4 * pBitCompleteMinuteBuffer[23] + 8 * pBitCompleteMinuteBuffer[24] + 10 * pBitCompleteMinuteBuffer[25] + 20 * pBitCompleteMinuteBuffer[26] + 40 * pBitCompleteMinuteBuffer[27];
	sum_min = pBitCompleteMinuteBuffer[21] +     pBitCompleteMinuteBuffer[22] +     pBitCompleteMinuteBuffer[23] +     pBitCompleteMinuteBuffer[24] +      pBitCompleteMinuteBuffer[25] +      pBitCompleteMinuteBuffer[26] +      pBitCompleteMinuteBuffer[27];
	bit_P1  = pBitCompleteMinuteBuffer[28];

	val_h  = pBitCompleteMinuteBuffer[29] + 2 * pBitCompleteMinuteBuffer[30] + 4 * pBitCompleteMinuteBuffer[31] + 8 * pBitCompleteMinuteBuffer[32] + 10 * pBitCompleteMinuteBuffer[33] + 20 * pBitCompleteMinuteBuffer[34];
	sum_h  = pBitCompleteMinuteBuffer[29] +     pBitCompleteMinuteBuffer[30] +     pBitCompleteMinuteBuffer[31] +     pBitCompleteMinuteBuffer[32] +      pBitCompleteMinuteBuffer[33] +      pBitCompleteMinuteBuffer[34];
	bit_P2 = pBitCompleteMinuteBuffer[35];

	val_d = pBitCompleteMinuteBuffer[36] + 2 * pBitCompleteMinuteBuffer[37] + 4 * pBitCompleteMinuteBuffer[38] + 8 * pBitCompleteMinuteBuffer[39] + 10 * pBitCompleteMinuteBuffer[40] + 20 * pBitCompleteMinuteBuffer[41];
	sum_d = pBitCompleteMinuteBuffer[36] +     pBitCompleteMinuteBuffer[37] +     pBitCompleteMinuteBuffer[38] +     pBitCompleteMinuteBuffer[39] +      pBitCompleteMinuteBuffer[40] +      pBitCompleteMinuteBuffer[41];

	val_wd = pBitCompleteMinuteBuffer[42] + 2 * pBitCompleteMinuteBuffer[43] + 4 * pBitCompleteMinuteBuffer[44];
	sum_wd = pBitCompleteMinuteBuffer[42] +     pBitCompleteMinuteBuffer[43] +     pBitCompleteMinuteBuffer[44];

	val_m = pBitCompleteMinuteBuffer[45] + 2 * pBitCompleteMinuteBuffer[46] + 4 * pBitCompleteMinuteBuffer[47] + 8 * pBitCompleteMinuteBuffer[48] + 10 * pBitCompleteMinuteBuffer[49];
	sum_m = pBitCompleteMinuteBuffer[45] +     pBitCompleteMinuteBuffer[46] +     pBitCompleteMinuteBuffer[47] +     pBitCompleteMinuteBuffer[48] +      pBitCompleteMinuteBuffer[49];

	val_y  = year_offset + pBitCompleteMinuteBuffer[50] + 2 * pBitCompleteMinuteBuffer[51] + 4 * pBitCompleteMinuteBuffer[52] + 8 * pBitCompleteMinuteBuffer[53] + 10 * pBitCompleteMinuteBuffer[54] + 20 * pBitCompleteMinuteBuffer[55] + 40 * pBitCompleteMinuteBuffer[56] + 80 * pBitCompleteMinuteBuffer[57];
	sum_y  =               pBitCompleteMinuteBuffer[50] +     pBitCompleteMinuteBuffer[51] +     pBitCompleteMinuteBuffer[52] +     pBitCompleteMinuteBuffer[53] +      pBitCompleteMinuteBuffer[54] +      pBitCompleteMinuteBuffer[55] +      pBitCompleteMinuteBuffer[56] +      pBitCompleteMinuteBuffer[57];
	bit_P3 = pBitCompleteMinuteBuffer[58];

	//Check for parity and skip if error found
	checksum_P1 = sum_min + bit_P1;
	checksum_P2 = sum_h + bit_P2;
	checksum_P3 = sum_d + sum_wd + sum_m + sum_y + bit_P3;

	if (checksum_P1 >0 && checksum_P2 >0 && checksum_P3 >0){
		while (checksum_P1 >= 2){ //Fast modulus operation
			checksum_P1 -= 2;
		}
		while (checksum_P2 >= 2){ //Fast modulus operation
			checksum_P2 -= 2;
		}
		while (checksum_P3 >= 2){ //Fast modulus operation
			checksum_P3 -= 2;
		}
	}

	if ((checksum_P1 + checksum_P2 + checksum_P3) != 0){

		strcpy(dateTimeString, "At least one parity failed!");
		return;
	}

	//Generate String for Output
#ifdef TI_COMPILER_USED								//Hours
	
	stringBuf_a2i = stoa(val_h);

	if(stringBuf_a2i[1]=='\r'){			//format String
		stringBuf_a2i = { '0', stringBuf_a2i[0], 0};
	}
	else{
		stringBuf_a2i = { stringBuf_a2i[0], stringBuf_a2i[1], 0 };
	}

#else
	_snprintf(stringBuf_a2i, 2, "%.2d", val_h);
#endif /*TI_COMPILER_USED*/	

	strcat(stringBuf_out, stringBuf_a2i);

	strcat(stringBuf_out, ":");
	
#ifdef TI_COMPILER_USED								//Minutes

	stringBuf_a2i = stoa(val_min);

	if (stringBuf_a2i[1] == '\r'){			//format String
		stringBuf_a2i = { '0', stringBuf_a2i[0], 0 };
	}
	else{
		stringBuf_a2i = { stringBuf_a2i[0], stringBuf_a2i[1], 0 };
	}

#else
	_snprintf(stringBuf_a2i, 2, "%.2d", val_min);
#endif /*TI_COMPILER_USED*/

	strcat(stringBuf_out, stringBuf_a2i);

	strcat(stringBuf_out, " Uhr, ");

	switch (val_wd) {								//Weekday
	case 1: strcat(stringBuf_out, "Montag ");
		break;
	case 2: strcat(stringBuf_out, "Dienstag ");
		break;
	case 3: strcat(stringBuf_out, "Mittwoch ");
		break;
	case 4: strcat(stringBuf_out, "Donnerstag ");
		break;
	case 5: strcat(stringBuf_out, "Freitag ");
		break;
	case 6: strcat(stringBuf_out, "Samstag ");
		break;
	case 7: strcat(stringBuf_out, "Sonntag ");
		break;
	}

#ifdef TI_COMPILER_USED								//Day

	stringBuf_a2i = stoa(val_d);

	if (stringBuf_a2i[1] == '\r'){			//format String
		stringBuf_a2i = { '0', stringBuf_a2i[0], 0 };
	}
	else{
		stringBuf_a2i = { stringBuf_a2i[0], stringBuf_a2i[1], 0 };
	}

#else
	_snprintf(stringBuf_a2i, 2, "%.2d", val_d);
#endif /*TI_COMPILER_USED*/

	strcat(stringBuf_out, stringBuf_a2i);

	strcat(stringBuf_out, ".");

#ifdef TI_COMPILER_USED								//Month

	stringBuf_a2i = stoa(val_m);

	if (stringBuf_a2i[1] == '\r'){			//format String
		stringBuf_a2i = { '0', stringBuf_a2i[0], 0 };
	}
	else{
		stringBuf_a2i = { stringBuf_a2i[0], stringBuf_a2i[1], 0 };
	}

#else
	_snprintf(stringBuf_a2i, 2, "%.2d", val_m);
#endif /*TI_COMPILER_USED*/

	strcat(stringBuf_out, stringBuf_a2i);

	strcat(stringBuf_out, ".");

#ifdef TI_COMPILER_USED								//Year

	stringBuf_a2i = stoa(val_y);
											//format String
	stringBuf_a2i = { stringBuf_a2i[0], stringBuf_a2i[1], stringBuf_a2i[2], stringBuf_a2i[3], 0 };

#else
	_snprintf(stringBuf_a2i, 4, "%.4d", val_y);
#endif /*TI_COMPILER_USED*/

	strcat(stringBuf_out, stringBuf_a2i);

	strncpy(dateTimeString, stringBuf_out, DATE_TIME_STRING_LEN);
	
	minBufind = 0;//TB 17.06.15 eingebaut
	return;
}