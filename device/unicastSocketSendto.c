/**
 * \file unicastSocketSendto.c
 * \brief Unicast Socket to send
 * \author Gautier.B
 * \version 0.1
 * \date 2 August 2007
 *
 * Program to send data in unicast via sockets
 */
#include "unicastSocketSendto.h"

/* gettimeofday */

/**
 * \fn void init_unisendto(void)
 * \brief  Initializes the unicast socket
 * \param nothing
 * \return  nothing.
 */
void init_unisendto(void)
{
#ifdef _WIN32

	WSADATA wsad; //Structure required to activate sockets
	if (WSAStartup(2,&wsad) != 0)
	{
		printf("Error n°%d : Error during initialization.\n", SOCKET_ERRNO);
		exit(errno);
	}
#endif
}

/**
 * \fn int init_connection_unisendto(int port, char *dest_ip, struct sockaddr_in * sDest)
 * \brief  initializes the connection of the unicast socket
 * \param nstruct sockaddr_in* sDest, structure of the multicast address; int port, port of the multicast connection; char *dest_ip, unicast ip adress
 * \return  int socket_id
 */
int init_connection_unisendto(int port, char *dest_ip, struct sockaddr_in * sDest)
{

	int socket_id;
	//Initialization of the UDP socket
	socket_id = socket(AF_INET, SOCK_DGRAM,0);
	if(socket_id == -1)
	{
		printf("Erreur n%d : Unable to initialize socket.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		return SOCKET_ERRNO;
	}

	// initialization of the destination structure
	sDest->sin_family = AF_INET;
	sDest->sin_port = htons(port);
	sDest->sin_addr.s_addr = inet_addr(dest_ip);

	return socket_id;

}

/**
 * \fn int write_unisendto(SOCKET socket_id, struct sockaddr_in * sDest, char *message)
 * \brief  allows to write on buffer send by the socket
 * \param struct sockaddr_in *sin, structure of address; SOCKET msock; char *message, request to send
 * \return  nbytes.
 */
int write_unisendto(SOCKET socket_id, struct sockaddr_in * sDest, char *message)
{
	int iResult = 0;  // variable to perform tests

	// Sending the message
	iResult = sendto(socket_id, message, strlen(message)+1, 0, (struct sockaddr*)sDest, sizeof(*sDest));
	if (iResult == -1)
	{
		printf("Error n%d : Unable to send message.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		closesocket(socket_id);
		WSACleanup();
		return SOCKET_ERRNO;
	}

	return iResult;
}

/**
 * \fn void end_connection_unisendto(void)
 * \brief  close the socket
 * \param SOCKET sock
 * \return  nothing.
 */
void end_connection_unisendto(SOCKET sock)
{
	if (closesocket(sock) != 0)
	{
		printf("Error closing the socket.\n");
		exit(SOCKET_ERRNO);

	}
}
/**
 * \fn void end_unisendto(void)
 * \brief  erases the contents of the unicast socket
 * \param nothing
 * \return  nothing.
 */
void end_unisendto(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}
