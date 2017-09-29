/**
 * \file DecToBin.h
 * \brief Header Conversion Program
 * \author Gautier.B
 * \version 0.1
 * \date 22 August 2017
 *
 * header of the program for converting a decimal number to a binary number
 */
#ifndef DECTOBIN_H_
#define DECTOBIN_H_

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

void dec2bin(int val, int bin[8], int ex);
char *ToBinary8(int nombreNatif);
char *ToBinary32(int nombreNatif);
char *ConvertIPdec2bin(char IPbinaire[50]);

#endif /* DECTOBIN_H_ */
