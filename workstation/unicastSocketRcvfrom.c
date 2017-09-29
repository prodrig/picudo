/**
 * \file unicastSocketRcvfrom.c
 * \brief Unicast socket to receive from
 * \author Gautier.B
 * \version 0.1
 * \date 2 August 2007
 *
 * Program to receive datas in unicast  by sockets
 *
 */
#include "unicastSocketRcvfrom.h"

char msg_rcv[1024]; //message received
/* gettimeofday */


/**
 * \fn void init_unircvfr(void)
 * \brief  Initializes the unicast socket
 * \param nothing
 * \return  nothing.
 */
void init_unircvfr(void)
{
	WSADATA wsad; //Structure required to activate sockets
#ifdef _WIN32
	if (WSAStartup (MAKEWORD(1,1), &wsad) != 0)
		{
			printf("Error n°%d : Error during initialization.\n", SOCKET_ERRNO);
		    exit(SOCKET_ERRNO);
		}

#endif
}
/**
 * \fn int init_connection_unircvfr(struct sockaddr_in sDest, int PORT, char *uni_ip)
 * \brief  initializes the connection of the unicast socket
 * \param nstruct sockaddr_in* sDest, structure of the unicast address; int port, port of the unicast connection; char *uni_ip, unicast ip adress
 * \return  int socket_id
 */
int init_connection_unircvfr(struct sockaddr_in sDest, int PORT, char *uni_ip)
{
	    int socket_id;

		//Initialization of the UDP socket
		socket_id = socket(AF_INET, SOCK_DGRAM,0);
		if(socket_id == -1)
		{
			printf("Error n%d : Unable to initialize socket.\n",SOCKET_ERRNO);
			//exit(SOCKET_ERRNO);
			return SOCKET_ERRNO;
		}

		// initialization of the destination structure
		sDest.sin_family = AF_INET;
		sDest.sin_port = htons(PORT);
		sDest.sin_addr.s_addr = inet_addr(uni_ip);

		/* bind to receive address */
		if (bind(socket_id, (struct sockaddr *)&sDest, sizeof(sDest)) < 0)
		{
			printf("Error n%d : bind() failed:\n", SOCKET_ERRNO);
			//exit(SOCKET_ERRNO);
			return SOCKET_ERRNO;

		}
		return socket_id;
}
/**
 * \fn int read_unircvfr(struct sockaddr_in sDest, SOCKET socket_id)
 * \brief  allows to read the contents of the data sent by the unicast socket
 * \param struct sockaddr_in sDest, structure of address; SOCKET socket_id
 * \return  iResult.
 */
int read_unircvfr(struct sockaddr_in sDest, SOCKET socket_id)
{

	int iResult = 0;  // variable to perform tests
	int sizeSdest = sizeof(sDest); //Size of the receiving structure


	// Receiving the message
	iResult = recvfrom(socket_id, msg_rcv, sizeof(msg_rcv), 0, (struct sockaddr *)&sDest, &sizeSdest);
    msg_rcv[iResult]='\0'; // recvfrom renvoie le nombre d'octet lu!

	if (iResult == -1)
	{
		printf("Error n%d : Unable to receive message.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		closesocket(socket_id);
		WSACleanup();
		return SOCKET_ERRNO;
	}

	msg_rcv[iResult] = '\0';

	// Display of the message without interference
	printf(">>%s\n", msg_rcv);

	return iResult;
}

/**
 * \fn void end_unircvfr(void)
 * \brief  erases the contents of the unicast socket
 * \param nothing
 * \return  nothing.
 */
void end_unircvfr(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}
/**
 * \fn void end_connection_unircvfr(SOCKET sock)
 * \brief  close the socket
 * \param SOCKET sock
 * \return  nothing.
 */
void end_connection_unircvfr(SOCKET sock)
{
	if (closesocket(sock) != 0)
	{
		printf("Error closing the socket.\n");
		exit(SOCKET_ERRNO);
	}
}

char *get_unimsg(void)
{
	return msg_rcv;
}



