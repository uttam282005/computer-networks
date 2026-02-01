#ifndef SERVER
#define SERVER

#define PORT "8080"
#define BACKLOG 10


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>          // getaddrinfo, freeaddrinfo
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
#include <arpa/inet.h>      // inet_ntop, inet_pton


#endif // SERVER
