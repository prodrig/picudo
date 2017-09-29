/**
 * \file appClient.c
 * \brief Program to simulate a client application
 * \author Gautier.B
 * \version 0.1
 * \date 15 August 2007
 *
 * Program to allow the client to choose the address and the port on which
 * he wants to communicate in unicast with the devices.
 *
 */

#include "appClient.h"
#include "DecToBin.h"


int fpurge(FILE *f);


char *requestApp(char req[52])
{
	char * request = NULL,
			*bnum=NULL,
			*bnum1=NULL;
	request = req;

	char IPbinaire[50] = {0},
			uni_port[9] = {0},
			proto_id[15] = {0},
			uni_addr[35] = {0};
			/*tsp =NULL;*/

	int proto = 0,
			prt = 0;

	while(1)
	{
		printf("\nType one of the following commands:\n"
				"0 --> Discovery protocol (request)\n"
				"1 --> Start command\n"
				"2 --> Stop command\n");
			/*	"3 --> Timestamp update\n");*/
		scanf ("%1d", &proto);
		fpurge(stdin);
		bnum = ToBinary8(proto);
		if (proto<3)
		{
			// Contrôle nom ok - Si ok, on sort de la boucle
			if (sscanf(bnum,"%s", proto_id) == 1)
			{
				strcpy(request, proto_id);
				break;
			}
		}
		// Saisie incorrecte
		printf("Incorrect input - try again\n");
	}
	if (proto==0){
		strcat(request, " ");
		while(1)
		{
			printf("\nChoose the worskation unicast channel's IP address (AA.BB.CC.DD):\n");
			/*fgets(saisie2,15, stdin);
			fpurge(stdin);*/
			if (sscanf(ConvertIPdec2bin(IPbinaire),"%s", uni_addr) == 1)
			{
				strcat(request, uni_addr);
				strcat(request, " ");
				break;
			}
			printf("Incorrect input - try again!\n");

		}

		while(1)
		{
			printf("\nChoose the worskation unicast channel's IP port(EE):");
			scanf ("%2d", &prt);
			fpurge(stdin);
			bnum1 = ToBinary8(prt);
			if (sscanf(bnum1,"%s", uni_port) == 1)
			{
				strcat(request, uni_port);
				strcat(request, "\0");
				break;
			}
			printf("Incorrect input - try again!\n");

		}

	}/*
	if (proto==3)
	{
		strcat(request, " ");
		time_t timestamp = time(NULL);
		tsp = ToBinary32(timestamp);
		strcat(request, tsp);
	}*/

	printf("\nDiscovery protocol (request): %s\n",req);
	return req;
}



/*Returns 0 if OK, a negative value if EOF.*/
int fpurge(FILE *f)
{
	int c;
	while((c=fgetc(f))!=EOF && c!='\n')
	{ }
	return (c==EOF ? -1 : 0);
}
