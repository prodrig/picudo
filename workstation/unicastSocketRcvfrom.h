/**
 * \file unicastSocketRcvfrom.h
 * \brief Header of unicastSocketRcvfrom.c
 * \author Gautier.B
 * \version 3
 * \date 7 August 2007
 *
 * Header of the program to receive datas in unicast by sockets
 */
#ifndef SOCKET_H
#define	SOCKET_H

#ifdef _WIN32

#include <stdio.h>
#include <winsock.h>

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

void init_unircvfr(void);
void end_unircvfr(void);
void end_connection_unircvfr(SOCKET sock);
int init_connection_unircvfr(struct sockaddr_in  addr, int PORT, char *GROUP_IP);
int read_unircvfr(struct sockaddr_in sin, SOCKET msock);
char *get_unimsg();

#endif	/* SOCKET_H */
