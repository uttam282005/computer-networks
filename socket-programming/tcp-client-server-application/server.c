#include "server.h"
#include <sys/socket.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

void *get_client_addr(struct sockaddr *s) {
  if (s->sa_family == AF_INET) {
    return &((struct sockaddr_in *)s)->sin_addr;
  }

  return &((struct sockaddr_in6 *)s)->sin6_addr;
}

void signchild_handler(int s) {
  (void)s;

  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;

  errno = saved_errno;
}

int main() {
  int yes = 1;
  int sock_fd, newsock_fd, rv;
  char client_ip[INET6_ADDRSTRLEN];
  struct addrinfo *res, hints, *p;
  struct sockaddr_storage client_info;
  struct sigaction sa;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(0);
  }

  for (p = res; p != NULL; p = p->ai_next) {
    if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("socket");
      continue;
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==
        -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sock_fd, p->ai_addr, p->ai_addrlen) != 0) {
      close(sock_fd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(res);

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(sock_fd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  sa.sa_handler = signchild_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  while (1) {
socklen_t addr_size = sizeof client_info;
    newsock_fd = accept(sock_fd, (struct sockaddr *) &client_info, &addr_size);
    if (newsock_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(client_info.ss_family,get_client_addr((struct sockaddr *)&client_info), client_ip,
              (socklen_t)sizeof client_ip);
    printf("server: got connection from %s\n", client_ip);

    if (fork() == 0) {
      close(sock_fd);
      if (send(newsock_fd, "Hello, World!", 13, 0) == -1) 
        perror("send");
      close(newsock_fd);
      exit(0);
    }
    close(newsock_fd);
  }

  return 0;
}
