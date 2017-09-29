/*
 * DecToBin.h
 *
 *  Created on: 21 août 2017
 *      Author: Aymeric
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
