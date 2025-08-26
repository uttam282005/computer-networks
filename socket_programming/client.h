#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, malloc, free
#include <string.h>     // memset, memcpy, strlen
#include <unistd.h>     // close, read, write
#include <sys/types.h>  // data types for sockets
#include <sys/socket.h> // socket, connect, send, recv
#include <netinet/in.h> // sockaddr_in, htons, htonl
#include <arpa/inet.h>  // inet_pton, inet_ntoa
#include <netdb.h>      // gethostbyname, struct hostent

#endif // CLIENT_H_
