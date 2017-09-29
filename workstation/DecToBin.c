/**
 * \file DecToBin.cpp
 * \brief Conversion Program
 * \author Gautier.B
 * \version 0.1
 * \date 22 August 2017
 *
 * Program for converting a decimal number to a binary number
 */

#include "DecToBin.h"

/**
 * \fn void dec2bin(int val, int bin[8], int ex)
 * \brief Depending on the function "ConvertIPdec2bin",
 * this function converts each decimal number of the IP
 * address into a binary number
 *
 * \param int val, the value of the decimal number; int bin[8], array of int
 * containing the number in binary; int ex, maximum length of binary array;
 * \return  nothing.
 */

void dec2bin(int val, int bin[8], int ex)
{
	int n = (int)pow(2, (double)ex);
	if(val >= n)
	{
		bin[ex] = 1;
		if(ex>0)
			dec2bin(val - n, bin, ex - 1);
	}
	else
	{
		bin[ex] = 0;
		if(ex>0)
			dec2bin(val, bin, ex - 1);
	}

}

/**
 * \fn  char *ConvertIPdec2bin(char IPbinaire[50])
 * \brief Function allowing the user to type the IP address
 * manually from the console, and to convert this address into binary address
 *
 * \param char IPbinaire[50], array of char which will contain the new binary address IP
 * \return  IPbinaire.
 */

char *ConvertIPdec2bin(char IPbinaire[50])
{


	int i=0, j=0;
	int decimal[4] = {0};
	int binaire[4][8] = {{0}};
	const char convert[4][8] = {{'\0'}}; ;

	do
	{
		scanf("%d", &decimal[0]);
		scanf("%d", &decimal[1]);
		scanf("%d", &decimal[2]);
		scanf("%d", &decimal[3]);
	}while(decimal[0] > 255 || decimal[1] > 255 || decimal[2] > 255 || decimal[3] > 255);

	printf("\nAdresse decimale : %d.%d.%d.%d\n", decimal[0], decimal[1], decimal[2], decimal[3]);
	for(i=0; i<4; i++)
		dec2bin(decimal[i], binaire[i], 7);

	for(i=0; i<4; i++)
	{
		for(j=7; j>=0; j--)
		{
			sprintf((char*)convert, "%d", binaire[i][j]);
			strcat(IPbinaire,(char*)convert);

		}
		if(i!=3){

			strcat(IPbinaire,"\0");
		}

	}

	return IPbinaire;
}

/**
 * \fn  char *ToBinary8(int nombreNatif)
 * \brief Function to convert an 8-bit int number , into a binary number of 8 bits
 *
 * \param int nombreNatif, number to convert
 * \return  BinaryChar8.
 */
char *ToBinary8(int nombreNatif)
{
	static char buffer [1+sizeof (int)*8] = { 0 };
	char *BinaryChar8=buffer-1+sizeof (int)*8;
	*BinaryChar8 = 0;

	int iBit;
	for(iBit=0 ; iBit<8; iBit++)
	{
		int valeurBit = nombreNatif & 0x80;
		nombreNatif <<= 1; //Shifts the number of one bit to the left

		if(valeurBit==0)
			strcat(BinaryChar8,"0");
		else
			strcat(BinaryChar8,"1");
	}
	return BinaryChar8;
}

/**
 * \fn  char *ToBinary32(int nombreNatif)
 * \brief Function to convert an 32bits - int number, into a binary number of 32 bits
 *
 * \param int nombreNatif, number to convert
 * \return  BinaryChar32.
 */

char *ToBinary32(int nombreNatif)
{
	static char buffer [1+sizeof (int)*32] = { 0 };
	char *BinaryChar32=buffer-1+sizeof (int)*32;

	int iBit;
	for(iBit=0 ; iBit<32 ; iBit++)
	{
		int valeurBit = nombreNatif & 0x80000000;
		nombreNatif <<= 1; //Shifts the number of one bit to the left

		if(valeurBit==0)
			strcat(BinaryChar32,"0");
		else
			strcat(BinaryChar32,"1");
	}
	return BinaryChar32;
}



