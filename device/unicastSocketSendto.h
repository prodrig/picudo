/**
 * \file unicastSocketSendto.h
 * \brief Header of unicastSocketSendto.h
 * \author Gautier.B
 * \version 2
 * \date 2 August 2007
 *
 * Header of the program to send datas in unicast  by sockets
 */
#ifndef SOCKET_H
#define	SOCKET_H

#ifdef _WIN32

#include <stdio.h>
#include <winsock.h>
#include  <inaddr.h>

#else

#include <stdio.h>          /* printf(), snprintf() */
#include <stdlib.h>         /* strtol(), exit() */
#include <sys/types.h>
#include <sys/socket.h>     /* socket(), setsockopt(), bind(),
                                         recvfrom(), sendto() */
#include <errno.h>          /* perror() */
#include <netinet/in.h>     /* IPPROTO_IP, sockaddr_in, htons(),
                                         htonl() */
#include <arpa/inet.h>      /* inet_addr() */
#include <unistd.h>         /* fork(), sleep() */
#include <sys/utsname.h>    /* uname() */
#include <string.h>         /* memset() */
#include  <inaddr.h>


#endif

#ifdef _WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif

void init_unisendto(void);
int init_connection_unisendto(int port, char *dest_ip, struct sockaddr_in * sDest);
int write_unisendto( SOCKET socket_id, struct sockaddr_in* sDest, char *message);
void end_connection_unisendto(SOCKET sock);
void end_unisendto(void);

#endif	/* SOCKET_H */

