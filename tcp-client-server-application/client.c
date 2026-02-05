#include "client.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXDATALEN 100

void *get_addr(struct addrinfo *ai) {
  if (ai->ai_family == AF_INET) {
    return &((struct sockaddr_in *)ai->ai_addr)->sin_addr;
  } else {
    return &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr;
  }
}

int main(int argc, char *argv[]) {
  int rv, sockfd, numbytes;
  char ip[INET6_ADDRSTRLEN];
  struct sockaddr addr;
  char buf[MAXDATALEN];

  if (argc != 2) {
    fprintf(stderr, "usage client: need ip to connect");
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints, *res, *p;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(argv[1], PORT, &hints, &res)) == -1) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  for (p = res; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1)) {
      perror("socket");
      continue;
    }

    inet_ntop(p->ai_family, get_addr(p), ip, sizeof ip);

    printf("attempting to connect to %s\n", ip);
    if ((rv = connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
      perror("connect");
      close(sockfd);
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "failed to connect");
    exit(EXIT_FAILURE);
  }

  inet_ntop(p->ai_family, get_addr(p), ip, sizeof ip);
  printf("connected to ip %s", ip);

  freeaddrinfo(res); // all done with this structure
  if ((numbytes = recv(sockfd, buf, MAXDATALEN - 1, 0)) == -1) {
    perror("recv");
    exit(1);
  }

  buf[numbytes] = '\0';

  printf("client: received '%s'\n", buf);
}
