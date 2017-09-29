/**
 * \file multiSocketSendto.h
 * \brief Header of multiSocketSendto.c
 * \author Gautier.B
 * \version 3
 * \date 7 August 2007
 *
 * Header of the program to send datas in multicast by sockets
 */
#ifndef MULTISOCKETSENDTO_H_
#define MULTISOCKETSENDTO_H_


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
#include <stdint.h>

#endif
#define BUF_SIZE 1024
#ifdef _WIN32
#define SOCKET_ERRNO	WSAGetLastError()
#else
#define SOCKET_ERRNO	errno
#endif

#define MSGBUFSIZE 1024



void init_multisendto(void);
int init_connection_multisendto (struct sockaddr_in *addr, int PORT, char *GROUP_IP);
int write_multisendto( SOCKET msock, struct sockaddr_in *addr, char *message);
void end_multisendto(void);
void end_connection_multisendto(SOCKET sock);

#endif /* MULTISOCKETSENDTO_H_ */
