/**
 * \file multiSocketRcvfrom.h
 * \brief Header of multiSocketRcvfrom.c
 * \author Gautier.B
 * \version 2
 * \date 2 August 2007
 *
 * Header of the program to receive datas in multicast  via sockets
 */
#ifndef MULTISOCKETRCVFROM_H_
#define MULTISOCKETRCVFROM_H_

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

#endif

#ifdef _WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif

void init_multircv(void);
void end_multircv(void);
void end_connection_multircv(SOCKET sock);
int init_connection_multircv( struct sockaddr_in* addr, int PORT, char *GROUP_IP);
int read_multircv( struct sockaddr_in* sin, SOCKET msock);
char *getReqMulti(void);

#endif /* MULTISOCKETRCVFROM_H_ */
