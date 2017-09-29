/**
 * \file device.c
 * \brief Program to simulate a device
 * \author Gautier.B
 * \version 0.1
 * \date 2 August 2007
 *
 * Device behavior simulation program, so that it responds correctly to the instructions of the workstation.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include <ctype.h>
#include "unicastSocketSendto.h"
#include "multiSocketRcvfrom.h"
#include "ParamDevice.h"

#include "DecToBin.h"
#include "BinToDec.h"

#define PORT1 12345
#define GROUP_IP "225.0.0.37"

#define PROTOCOL_ID_DISCOVERY    0x00
#define PROTOCOL_ID_START_CMD    0x01
#define PROTOCOL_ID_STOP_CMD     0x02
#define PROTOCOL_ID_TIMESTAMP    0x03

#define NUM_DEV 1234

pthread_cond_t condition = PTHREAD_COND_INITIALIZER; /* Creation of the condition */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* Creation of the mutex */


void *rvc_request(void *arg);
void *send_uni(void *arg);


char audiodata[2048]=  {0};
char *rq; volatile int bool = 1;
int socket_id = 0, socket_id1 = 0;
int usock = 0;

char ipdecimal[40] = {0};
char *port={0};


struct ParamDevice devicePrototype;

/**
 * \fn int main (void)
 * \brief Program entry
 *
 * \return EXIT_SUCCESS - Normal stop of the program.
 */
int main(void)
{

	devicePrototype.nodeId= ToBinary8(NUM_DEV);
	devicePrototype.timestamp = ToBinary32(0);
	strcpy(devicePrototype.data,"111100001111000");
	devicePrototype.lengthData = strlen (devicePrototype.data);

	/* this variables are our references to the threads */


	pthread_t threadRcv;
	pthread_t threadSd;


	// create a first thread which executes rvc_request(void *arg)
	pthread_create(&threadRcv, NULL, rvc_request,NULL);
	// create a second thread which executes send_uni(void *arg)
	pthread_create(&threadSd, NULL, send_uni,NULL);

	//wait for the first thread to finish
	pthread_join(threadRcv, NULL);
	//wait for the second thread to finish
	pthread_join(threadSd, NULL);

	end_connection_multircv(usock);
	end_connection_unisendto(socket_id);
	end_connection_unisendto(socket_id1);

	end_multircv();
	end_unisendto();


	return EXIT_SUCCESS;

}


/**
 * \fn void *rvc_request(void *arg)
 * \brief thread to receive the workstation's request
 *
 * \return NULL
 */
void *rvc_request(void *arg)
{
	struct sockaddr_in *sin;
	char terme1[9]= {0};
	char terme2[9]= {0};
	char terme3[9]= {0};
	char terme4[9]= {0};

	init_multircv();

	usock = init_connection_multircv(sin, PORT1,GROUP_IP);


	while (1)
	{
		read_multircv(sin, usock);

		rq = getReqMulti();
		//separation of the IP address from the rest of the request
		sscanf(rq,"%*[^ ]%*c%[^ ]", devicePrototype.addUnicast);
		printf("IP:%s\n", devicePrototype.addUnicast);

		strncpy(terme1, devicePrototype.addUnicast, 8);
		strncpy(terme2, devicePrototype.addUnicast + 8, 8);
		strncpy(terme3, devicePrototype.addUnicast + 16, 8);
		strncpy(terme4, devicePrototype.addUnicast + 24, 8);

		//Conversion of the binary IP address to decimal IP address
		strcpy(ipdecimal,BinaryDecimalConverter8(terme1));
		strcat(ipdecimal, ".");
		strcat(ipdecimal,BinaryDecimalConverter8(terme2));
		strcat(ipdecimal, ".");
		strcat(ipdecimal,BinaryDecimalConverter8(terme3));
		strcat(ipdecimal, ".");
		strcat(ipdecimal,BinaryDecimalConverter8(terme4));
		printf("%s", ipdecimal);

		//separates and convert to decimal the port sent in the request
		char *pLastSlash = strrchr(rq, ' ');
		port = pLastSlash ? pLastSlash + 1 : rq;
		char p[3] = {0};
		strcpy(p,BinaryDecimalConverter8(port));
		devicePrototype.analogChannels = atoi(p);

		pthread_mutex_lock (&mutex); // The mutex is locked
		pthread_cond_signal (&condition); // The signal is output: condition fulfilled
		pthread_mutex_unlock (&mutex); //Unlock the mutex


	}

	pthread_exit(NULL);
	return NULL;
}

/**
 * \fn void *send_uni(void *arg)
 * \brief thread to send the device's answer and the device's data
 *
 * \return NULL
 */

void *send_uni(void *arg)
{
	int i =0;
	long int p = 0;
	char* fin = NULL;
	char protoID[9]={0};

	char audiodata[2048]=  {0};
	char * numberPacket, *ld;
	char* tpsstp;

	SOCKADDR_IN *sDest, *sDest1;

	init_unisendto();


	while(1)
	{
		pthread_mutex_lock(&mutex); //The mutex is locked
		pthread_cond_wait (&condition, &mutex); //The condition is expected to be fulfilled

		strncpy(protoID, rq, 8);
		p = strtol(protoID, &fin,8);

		socket_id = init_connection_unisendto(devicePrototype.analogChannels,ipdecimal, sDest);
		socket_id1 = init_connection_unisendto(devicePrototype.analogChannels,ipdecimal, sDest1);


		switch (p)
		{

		case PROTOCOL_ID_DISCOVERY: 		//behavior of the program in the case: ProtoID = 0x00


			//function to send the answer to the workstation
			write_unisendto(socket_id,sDest, rq);
			break;

		case PROTOCOL_ID_TIMESTAMP:			//behavior of the function in the case: ProtoID = 0x03

			tpsstp = strrchr(rq,' ')+1;
			strcpy(devicePrototype.timestamp, tpsstp);
			printf("Device's timestamp updated: %s", devicePrototype.timestamp);
			break;

		case PROTOCOL_ID_STOP_CMD: 		//behavior of the function in the case: ProtoID = 0x02

			bool = 0;
			break;

		case PROTOCOL_ID_START_CMD: 		//behavior of the function in the case: ProtoID = 0x01

			while (bool == 1 ){
				numberPacket = malloc(4);
				ld = malloc(32);

				strcpy(audiodata, devicePrototype.nodeId);
				strcat(audiodata, " ");
				strcat(audiodata, devicePrototype.timestamp);
				i++;
				numberPacket =  ToBinary8(i);
				strcat(audiodata, " ");
				strcat(audiodata, numberPacket);
				strcat(audiodata, " ");
				strcat(audiodata, port);
				strcat(audiodata, " ");
				ld= ToBinary8(devicePrototype.lengthData);
				strcat(audiodata, ld);
				strcat(audiodata, " ");
				strcat(audiodata,devicePrototype.data);
				strcat(audiodata, "\0");
				printf("%s\n", audiodata);

				write_unisendto(socket_id1, sDest1, audiodata);

				sleep(5);
			}
			break;
		}

		pthread_mutex_unlock(&mutex); // Mutex blocked
	}

	pthread_exit(NULL);
	return NULL;
}



