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

void get_ip(struct sockaddr_storage *sa, char *buf, size_t len) {
    inet_ntop(sa->ss_family, get_addr(sa), buf, len);
}

int get_listening_socket(void) {
    int yes = 1;
    int sockfd = -1;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int rv = getaddrinfo(NULL, PORT, &hints, &res);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;

        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }
        break;
    }

    freeaddrinfo(res);

    if (!p) {
        fprintf(stderr, "failed to bind\n");
        return -1;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void delete_from_pfds(struct pollfd *pfds, int *i, int *fd_count) {
    pfds[*i] = pfds[*fd_count - 1];
    (*fd_count)--;
    (*i)--;
}

void add_to_pfds(int newfd, struct pollfd **pfds, int *fd_size, int *fd_count) {
    if (*fd_count == *fd_size) {
        *fd_size *= 2;
        struct pollfd *tmp = realloc(*pfds, sizeof(**pfds) * (*fd_size));
        if (!tmp) {
            perror("realloc");
            exit(1);
        }
        *pfds = tmp;
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN;
    (*pfds)[*fd_count].revents = 0;
    (*fd_count)++;
}

void handle_new_connection(int listener, struct pollfd **pfds,
                           int *fd_count, int *fd_size) {
    struct sockaddr_storage client_addr;
    socklen_t addrlen = sizeof client_addr;
    int newfd = accept(listener, (struct sockaddr *)&client_addr, &addrlen);

    if (newfd == -1) {
        perror("accept");
        return;
    }

    add_to_pfds(newfd, pfds, fd_size, fd_count);

    char ip[INET6_ADDRSTRLEN];
    get_ip(&client_addr, ip, sizeof ip);
    printf("new connection from %s on fd %d\n", ip, newfd);
}

void handle_client(int listener, struct pollfd *pfds,
                   int *fd_count, int *i) {
    char buf[256];
    int client_fd = pfds[*i].fd;

    int nbytes = recv(client_fd, buf, sizeof buf, 0);
    if (nbytes <= 0) {
        if (nbytes == 0) {
            printf("connection closed: fd %d\n", client_fd);
        } else {
            perror("recv");
        }

        close(client_fd);
        delete_from_pfds(pfds, i, fd_count);
        return;
    }

    for (int j = 0; j < *fd_count; j++) {
        int dest_fd = pfds[j].fd;
        if (dest_fd != listener && dest_fd != client_fd) {
            send(dest_fd, buf, nbytes, 0);
        }
    }
}

int main(void) {
    int listener = get_listening_socket();
    if (listener == -1) {
        fprintf(stderr, "failed to create listening socket\n");
        exit(1);
    }

    int fd_size = 5;
    int fd_count = 1;

    struct pollfd *pfds = malloc(sizeof *pfds * fd_size);
    if (!pfds) {
        perror("malloc");
        exit(1);
    }

    pfds[0].fd = listener;
    pfds[0].events = POLLIN;

    puts("pollserver: waiting for connections...");

    while (1) {
        if (poll(pfds, fd_count, -1) == -1) {
            perror("poll");
            exit(1);
        }

        for (int i = 0; i < fd_count; i++) {
            if (pfds[i].revents & (POLLIN | POLLHUP)) {
                if (pfds[i].fd == listener) {
                    handle_new_connection(listener, &pfds,
                                          &fd_count, &fd_size);
                } else {
                    handle_client(listener, pfds,
                                  &fd_count, &i);
                }
            }
        }
    }
}

