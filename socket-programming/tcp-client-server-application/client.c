#include "client.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXDATALEN 100

void *get_addr(struct addrinfo *sa) {
  switch (sa->ai_family) {
  case AF_INET:
    return &((struct sockaddr_in *)sa)->sin_addr;
  case AF_INET6:
    return &((struct sockaddr_in6 *)sa)->sin6_addr;
  }

  return NULL;
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

  char *ip_to_conn = argv[1];
  struct addrinfo *hints, *res, *p;
  memset(hints, 0, sizeof *hints);
  hints->ai_protocol = SOCK_STREAM;
  hints->ai_family = AF_UNSPEC;

  if ((rv = getaddrinfo(ip_to_conn, PORT, hints, &res) != 0)) {
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
