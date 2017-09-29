/**
 * \file multiSocketSendto.c
 * \brief Program to send data by multicast socket
 * \author Gautier.B
 * \version 0.1
 * \date 2 August 2007
 *
 * Program to send datas in multicast  by sockets
 *
 */
#include "multiSocketSendto.h"

#define MSGBUFSIZE 1024
/**
 * \fn void init_multircv(void)
 * \brief  Initializes the multicast socket
 * \param nothing
 * \return  nothing.
 */
void init_multisendto(void)
{
	WSADATA WSAData; // Contains details of the Winsock implementation

#ifdef _WIN32
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
		{
			printf("Error n°%d : Error during initialization.\n", SOCKET_ERRNO);
		    exit(SOCKET_ERRNO);
		}

#endif
}
/**
 * \fn int init_connection_multisendto ( struct sockaddr_in* addr, int PORT, char *GROUP_IP)
 * \brief  initializes the connection of the multicast socket
 * \param nstruct sockaddr_in* addr, structure of the multicast address; int PORT, port of the multicast connection; char *GROUP_IP, group of multicast ip adress
 * \return  int msock
 */
int init_connection_multisendto (struct sockaddr_in *addr, int PORT, char *GROUP_IP){

	int msock;

	//create what looks like an ordinary UDP socket
	if ((msock=socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		printf("Error n%d : Unable to initialize socket.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		return SOCKET_ERRNO;
	}

	//set up destination address
	memset(addr,0,sizeof(*addr));

	addr->sin_family=AF_INET;
	addr->sin_addr.s_addr=inet_addr(GROUP_IP);
	addr->sin_port=htons(PORT);

	return msock;
}

/**
 * \fn int write_multisendto(SOCKET msock, struct sockaddr_in *addr, char *rq)
 * \brief  allows to write on buffer send by the multicast socket
 * \param struct sockaddr_in *sin, structure of address; SOCKET msock;char *rq,  request to send
 * \return  nbytes.
 */
int write_multisendto(SOCKET msock, struct sockaddr_in *addr, char *rq)
{

	if (sendto(msock, rq, strlen(rq)+1, 0, (struct sockaddr*) addr,
			sizeof(*addr) )<0)
	{
		printf("Error n%d : Unable to send message.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		return SOCKET_ERRNO;

	}
	return 0;
 }

/**
 * \fn void end_multisendto(void)
 * \brief  erases the contents of the multicast socket
 * \param nothing
 * \return  nothing.
 */
 void end_multisendto(void)
 {
 #ifdef WIN32
 	WSACleanup();
 #endif
 }
 /**
  * \fn  void end_connection_multisendto(SOCKET sock)
  * \brief  close the socket
  * \param SOCKET sock
  * \return  nothing.
  */
 void end_connection_multisendto(SOCKET sock)
 {
 	if (closesocket(sock) != 0)
 	{
 		printf("Error closing the socket.\n");
 		exit(SOCKET_ERRNO);
 	}
 }
