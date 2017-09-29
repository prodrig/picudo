/**
 * \file multiSocketRcvfrom.c
 * \brief Multicast Socket to receive data
 * \author Gautier.B
 * \version 0.1
 * \date 2 August 2007
 *
 * Program to receive datas in multicast  by sockets
 */

#include "multiSocketRcvfrom.h"

#define MSGBUFSIZE 55

char msgbuf[MSGBUFSIZE];

/**
 * \fn void init_multircv(void)
 * \brief  Initializes the multicast socket
 * \param nothing
 * \return  nothing.
 */

void init_multircv(void)
{
#ifdef _WIN32
	// Contains details of the
	// Winsock implementation
	WSADATA WSAData;
	// Initialize Winsock.
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
	{
		printf("Error n°%d : Error during initialization.\n", SOCKET_ERRNO);
		exit(SOCKET_ERRNO);
	}
#endif
}

/**
 * \fn void end_multircv(void)
 * \brief  erases the contents of the multicast socket
 * \param nothing
 * \return  nothing.
 */
void end_multircv(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}
/**
 * \fn void end_connection_multircv(SOCKET sock)
 * \brief  close the socket
 * \param SOCKET sock
 * \return  nothing.
 */
void end_connection_multircv(SOCKET sock)
{
	if (closesocket(sock) != 0)
	{
		printf("Error closing the socket.\n");
		exit(SOCKET_ERRNO);
	}
}

/**
 * \fn int init_connection_multircv( struct sockaddr_in* addr, int PORT, char *GROUP_IP)
 * \brief  initializes the connection of the multicast socket
 * \param nstruct sockaddr_in* addr, structure of the multicast address; int PORT, port of the multicast connection; char *GROUP_IP, group of multicast ip adress
 * \return  int msock
 */

int init_connection_multircv( struct sockaddr_in* addr, int PORT, char *GROUP_IP)
{
	int msock;
	struct ip_mreq mreq;
	u_int yes=1;


	//Create a socket
	if ((msock=socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		printf("Erreur n%d : Unable to initialize socket.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		return SOCKET_ERRNO;
	}

	// allow multiple sockets to use the same PORT number */
	if (setsockopt(msock,SOL_SOCKET,SO_REUSEADDR,(char*)&yes,sizeof(yes)) < 0)
	{
		printf("Erreur n%d : Reusing ADDR failed.\n",SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		return SOCKET_ERRNO;
	}

	// set up destination address
	memset(addr,0,sizeof(*addr));
	addr->sin_family=AF_INET;
	addr->sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
	addr->sin_port=htons(PORT);

	//bind to receive address
	if (bind(msock,(struct sockaddr *)addr,sizeof(*addr)) < 0)
	{
		printf("bind() failed: %d\n", WSAGetLastError());
		return 1;
	}

	// use setsockopt() to request that the kernel join a multicast group
	mreq.imr_multiaddr.s_addr=inet_addr(GROUP_IP);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if (setsockopt(msock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&mreq,sizeof(mreq)) < 0) {
		printf("Error n%d : Setsockopt failed:\n", SOCKET_ERRNO);
		//exit(SOCKET_ERRNO);
		return SOCKET_ERRNO;
	}
	return msock;
}
/**
 * \fn int read_multircv(struct sockaddr_in *sin, SOCKET msock)
 * \brief  allows to read the contents of the data sent by the multicast socket
 * \param struct sockaddr_in *sin, structure of address; SOCKET msock
 * \return  nbytes.
 */
int read_multircv( struct sockaddr_in *sin, SOCKET msock){
	// Read the message send
	int  nbytes;
	int addrlen=sizeof(*sin);

	if ((nbytes=recvfrom(msock,msgbuf,MSGBUFSIZE,0,
			(struct sockaddr *)sin,&addrlen)) < 0)
	{
		printf("Error n%d : Unable to receive message.\n",SOCKET_ERRNO);
		closesocket(msock);
		WSACleanup();
		return SOCKET_ERRNO;

	}
	msgbuf[nbytes]='\0';

	printf("nbytes>>%d\n", nbytes);
	printf(">>%s\n", msgbuf);

	return nbytes;

}




char *getReqMulti(void){

	return msgbuf;

}
