/**
 * \file workstation.c
 * \brief Workstation program
 * \author Gautier.B
 * \version 0.1
 * \date 3 August 2007
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
#include "unicastSocketRcvfrom.h"
#include "multiSocketSendto.h"
#include "BinToDec.h"
#include "appClient.h"
#include "DecToBin.h"

#define PORT1 12345

#define GROUP_IP "225.0.0.37"

#define PROTOCOL_ID_DISCOVERY    0x00
#define PROTOCOL_ID_START_CMD    0x01
#define PROTOCOL_ID_STOP_CMD     0x02
#define PROTOCOL_ID_TIMESTAMP    0x03



pthread_cond_t condition0 = PTHREAD_COND_INITIALIZER; /* Création of the condition0 */
pthread_mutex_t mutex0 = PTHREAD_MUTEX_INITIALIZER; /* Création of the mutex0 */

pthread_cond_t condition_tps = PTHREAD_COND_INITIALIZER; /* Création of the condition_tps */
pthread_mutex_t mutex_tps = PTHREAD_MUTEX_INITIALIZER; /* Création of the mutex_tps */

void *send_multi(void *arg);
void *rcv_uni(void *arg);
void *put_data_in_file(char *data);
//void *timestamp(void *arg);

char *rq;
int tmp_bool = 1;
char *time_rq = NULL;
char *tsp =NULL;

char ipdecimal[40] = {0};
int portUni = 0;
volatile int bool = 1;


SOCKET usock, usock1,msocket;

/**
 * \fn int main (void)
 * \brief Program entry
 *
 * \return EXIT_SUCCESS - Normal stop of the program.
 */

int main(void)
{

	//there variables are our references to the second thread
	pthread_t threadSendmulti;
	pthread_t threadRcv;


	//create a second thread which executes send_uni(void *arg)
	pthread_create(&threadSendmulti, NULL, send_multi, NULL);
	pthread_create(&threadRcv, NULL, rcv_uni, NULL);
	//pthread_create(&threadTimestamp, NULL, timestamp, NULL);


	// wait for the second thread to finish

	pthread_join(threadSendmulti, NULL);
	pthread_join(threadRcv, NULL);
	//pthread_join(threadTimestamp, NULL);




}
/**
void *timestamp(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_tps); // The mutex is locked
		pthread_cond_wait (&condition_tps, &mutex_tps); // The condition is expected to be fulfilled

		strcpy(time_rq, ToBinary8(3));
		strcat(time_rq, " ");
		time_t timestamp = time(NULL);
		tsp = ToBinary32(timestamp);
		strcat(time_rq, tsp);

		multiSocketSend (PORT1, GROUP_IP, time_rq);

		pthread_mutex_unlock(&mutex_tps); //Unlock the mutex

	}

}
 */

/**
 * \fn void *send_multi(void *arg)
 * \brief thread to send the workstation's request in multicast to all device
 *
 * \return NULL
 */

void *send_multi(void *arg)
{
	struct sockaddr_in *addr;

	char terme1[9]= {0};
	char terme2[9]= {0};
	char terme3[9]= {0};
	char terme4[9]= {0};
	char addrUni[35];

	init_multisendto();
	msocket = init_connection_multisendto (addr, PORT1, GROUP_IP);
	while(1)
	{
		char req[1024] = {0};

		rq = requestApp(req);

		printf("\nType 'S' to send the command ('Q' to exit): \r\n");
		char c = getchar();
		fpurge(stdin);
		if ((c=='s')||(c=='S'))
		{


			write_multisendto(msocket, addr, rq);


			printf("Sending successfully! (multicast)\n");

			sscanf(rq,"%*[^ ]%*c%[^ ]", addrUni);
			printf("IP:%s\n", addrUni);
			strncpy(terme1, addrUni, 8);
			strncpy(terme2, addrUni + 8, 8);
			strncpy(terme3, addrUni + 16, 8);
			strncpy(terme4, addrUni + 24, 8);

			strcpy(ipdecimal,BinaryDecimalConverter8(terme1));
			strcat(ipdecimal, ".");
			strcat(ipdecimal,BinaryDecimalConverter8(terme2));
			strcat(ipdecimal, ".");
			strcat(ipdecimal,BinaryDecimalConverter8(terme3));
			strcat(ipdecimal, ".");
			strcat(ipdecimal,BinaryDecimalConverter8(terme4));
			printf("%s", ipdecimal);

			char *pLastSlash = strrchr(rq, ' ');
			char *port = pLastSlash ? pLastSlash + 1 : rq;
			char pt[3] = {0};
			strcpy(pt,BinaryDecimalConverter8(port));
			portUni = atoi(pt);


			pthread_mutex_lock (&mutex0); /* lock the mutex */
			pthread_cond_signal (&condition0); /* The signal is output: condition fulfilled */
			pthread_mutex_unlock (&mutex0); /* Unlock the mutex */


			sleep(3);
		}
		else if((c=='q')||(c=='Q'))
		{
			break;
		}else
		{
			printf("\nTry Again! \r\n");
		}
	}
	pthread_exit(NULL);
	return NULL;
}

/**
 * \fn void *rcv_uni(void *arg)
 * \brief thread to receive the device's data in unicast
 *
 * \return NULL
 */


void *rcv_uni(void *arg)
{
	char protoID[9]={0};
	long int p = 0;
	char* fin = NULL;

	char data[1048]= {0};



	struct sockaddr_in addr, addr1;


	while (1)
	{
		pthread_mutex_lock(&mutex0); /*The mutex is locked */
		pthread_cond_wait (&condition0, &mutex0); /* The condition is expected to be fulfilled */

		end_connection_multisendto(msocket);
		end_multisendto();

		strncpy(protoID, rq, 8);
		p = strtol(protoID, &fin,8);

		switch (p)
		{
		case PROTOCOL_ID_DISCOVERY: //behavior of the program in the case: ProtoID = 0x00

			init_unircvfr();
			usock1 = init_connection_unircvfr(addr1 ,portUni, ipdecimal);
			if (read_unircvfr(addr1, usock1)==51)
			{
				printf("Answer: %s\n", get_unimsg());
				printf("\nDevice ready to be used!\n");
			}
			else
				printf("Device not connected\n");

			end_connection_unircvfr(usock1);
			end_unircvfr();

			break;

		case PROTOCOL_ID_START_CMD: //behavior of the program in the case: ProtoID = 0x01

			/*printf("\nUpdate timestamps? (O/N) \r\n");
						char c = getchar();
						fpurge(stdin);
						if ((c=='o')||(c=='O'))
							tmp_bool = 0;

						else if((c=='n')||(c=='N'))
							tmp_bool = 1;

						printf("%d",tmp_bool);*/
			do
			{
				/**if (tmp_bool==0)
				{
					pthread_mutex_lock (&mutex_tps); //lock the mutex
					pthread_cond_signal (&condition_tps); // The signal is output: condition fulfilled
					pthread_mutex_unlock (&mutex_tps); //Unlock mutex
				}*/


				init_unircvfr();
				usock = init_connection_unircvfr(addr ,portUni, ipdecimal);
				read_unircvfr(addr, usock);
				end_connection_unircvfr(usock);
				end_unircvfr();


				strcpy(data, get_unimsg());
				/*if (data !=)
				 *
				 */
				put_data_in_file(data);

			}while (bool ==1);
			break;

		case PROTOCOL_ID_STOP_CMD: //behavior of the program in the case: ProtoID = 0x03
			bool =0;
			printf("Stop Listening.\n");
			break;
		}
		pthread_mutex_unlock(&mutex0); /* Unlock le mutex */
	}

	pthread_exit(NULL); //Fin du thread
	return NULL;
}


/**
 * \fn void *put_data_in_file(char *data, int j)
 * \brief fonction to put datas in file
 *
 * \return NULL
 */

void *put_data_in_file(char *data)
{
	char name[15] = "data";

	FILE* fichier = NULL;

	strcat(name, ".dat");
	printf("%s",name);


	fichier = fopen(name, "a");

	if (fichier != NULL)
	{
		// We can read and write into the folder
		fputs(data, fichier);
		fclose(fichier);
	}
	else
	{
		// display error message
		printf("Impossible to open the file test.txt");
	}
	return NULL;
}
