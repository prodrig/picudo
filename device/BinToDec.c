/**
 * \file BinToDec.c
 * \brief Conversion Program
 * \author Gautier.B
 * \version 0.1
 * \date 22 August 2017
 *
 * Program for converting a binary number to a decimal number
 */

#include "BinToDec.h"

/**
 * \fn char *BinaryDecimalConverter(char BinaryChar[8])
 * \brief function of converting a char-type binary number to a char-type decimal number, decimalChar
 *
 * \param BinaryChar[8]  array of char for binary character to be filled.
 * \return  decimalChar char containing the new decimal number.
 */

char *BinaryDecimalConverter8(char BinaryChar[8])
{
	static char buffer [1+sizeof (int)*8] = { 0 };
	char *decimalChar=buffer-1+sizeof (int)*8;
	*decimalChar = 0;
	int DecimalInt = 0;
	for (int i = 0; i<strlen(BinaryChar); i++)
	{
		if (BinaryChar[i] == '1')
			DecimalInt = DecimalInt * 2 + 1;
		else if (BinaryChar [i] == '0')
			DecimalInt *= 2;
	}
	sprintf(decimalChar, "%d", DecimalInt);
	return decimalChar;
}
