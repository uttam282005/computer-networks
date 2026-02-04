#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "8080"
#define BACKLOG 10

void *get_addr(struct sockaddr_storage *sa) {
  if (sa->ss_family == AF_INET) {
    return &((struct sockaddr_in *)sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *)sa)->sin6_addr;
}

int get_listening_socket() {
  int yes = 1;
  int sockfd = -1;
  struct addrinfo hints, *res, *p;
  char server_ip[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int rv = getaddrinfo(NULL, PORT, &hints, &res);
  if (rv == -1) {
    perror("getaddrinfo");
    return 1;
  }

  for (p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("socket");
      continue;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    rv = bind(sockfd, p->ai_addr, p->ai_addrlen);
    if (rv == -1) {
      close(sockfd);
      perror("bind");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "failed to get a socket");
    return -1;
  }

  freeaddrinfo(res);
  if (listen(sockfd, BACKLOG) == -1) {
    return -1;
  }

  return sockfd;
}

void delete_from_pfds(struct pollfd *pfds, int *i, int *fd_count) {}

void add_to_pfds(int newfd, struct pollfd **pfds, int *fd_size, int *fd_count) {
  if (*fd_count == *fd_size) {
    *fd_size *= 2;
    *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
  }

  (*pfds)[*fd_count].fd = newfd;
  (*pfds)[*fd_count].events = POLLIN;
  (*pfds)[*fd_count].revents = 0;

  (*fd_count)++;
}

void *get_adr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &((struct sockaddr_in *) sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *) sa)->sin6_addr;
}

const char *get_ip(struct sockaddr_storage *sa) {
  char buf[INET6_ADDRSTRLEN];
  return inet_ntop(sa->ss_family, get_addr(sa), buf, sizeof buf);
}

void hanndle_new_connection(int listener, struct pollfd *pfds, int *fd_count,
                            int *fd_size) {
  struct sockaddr_storage client_info;
  int newfd;
  socklen_t addrlen = sizeof client_info;
  newfd = accept(listener, (struct sockaddr *)&client_info, &addrlen);

  if (newfd == -1) {
    perror("accept");
  } else {
    add_to_pfds(newfd, &pfds, fd_size, fd_count);

    printf("pollserver: new connection from %s on socket %d\n", get_ip(&client_info), newfd);
  }
}

void handle_client(int listener, struct pollfd *pfds, int *fd_count, int *i) {
  char buf[256];

  int client_sock = pfds[*i].fd;
  int nbytes = recv(client_sock, buf, sizeof buf, 0);
  if (nbytes <= 0) {
    if (nbytes == 0) {
      // connection closed by client
      printf("pollserver: connection closed by %d\n", client_sock);
    } else {
      perror("recv");
    }

    close(client_sock);
    delete_from_pfds(pfds, i, fd_count);
  } else {
    for (int j = 0; j < *fd_count; j++) {
      int dest_fd = pfds[j].fd;
      if (send(dest_fd, buf, nbytes, 0) == -1) {
        perror("send");
      }
    }
  }
}

void process_connection(int listener, struct pollfd **pfds, int *fd_count,
                        int *fd_size) {
  for (int i = 0; i < *fd_count; i++) {
    if ((*pfds[i]).revents & (POLLIN | POLLHUP)) {
      // ready to read
      int avalsock = (*pfds)[i].fd;
      if (avalsock == listener) {
        // new connection
        hanndle_new_connection(listener, *pfds, fd_count, &i);
      } else {
        handle_client(listener, *pfds, fd_count, &i);
      }
    }
  }
}

int main() {
  int listener;
  int fd_size = 5;
  int fd_count = 1;
  listener = get_listening_socket();
  if (listener == -1) {
    fprintf(stderr, "failed to obtain a listening socket");
    exit(EXIT_FAILURE);
  }
  struct pollfd *pfds = malloc(sizeof *pfds * fd_size);
  pfds[0].fd = listener;
  pfds[0].events = POLLIN;

  puts("pollserver: waiting for connections...");

  for (;;) {
    int poll_count = poll(pfds, fd_count, -1);

    if (poll_count == -1) {
      perror("poll");
      exit(1);
    }

    process_connection(listener, &pfds, &fd_count, &fd_size);
  }
}
