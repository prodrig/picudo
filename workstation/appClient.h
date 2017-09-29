/*
 * appClient.h
 *
 *  Created on: 15 août 2017
 *      Author: Aymeric
 */

#ifndef APPCLIENT_H_
#define APPCLIENT_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int fpurge(FILE *f);

char *requestApp(char *request);
char *getRequest(void);



#endif /* APPCLIENT_H_ */
