#include "server.h"
#include <stdio.h>

enum Status {
    SUCCESS = 0,
    FAILURE = -1,
};

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        error("Need port number.");
    }

    int server_fd, portno, newsocket_fd;
    enum Status status;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        error("Socket creation failed.");
    }

    portno = atoi(argv[1]);

    struct sockaddr_in server_addr, client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((status = bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) == FAILURE) {
        error("Port binding fails...");
    }

    listen(server_fd, 5);
    printf("Listening for connections...\n");
    socklen_t clientlen = sizeof(client_addr);

    if ((newsocket_fd = accept(server_fd, (struct sockaddr*) &server_addr, &clientlen)) == FAILURE) {
       error("Failed to accept the incoming connection....");
    }

    // receive file

    char buffer[255];
    FILE* f = fopen("glad_received.txt", "w");
    int words = 0;

    if ((status = read(newsocket_fd, &words, sizeof(int))) == FAILURE) {
        error("Error reading from the socket...");
    }

    while(words) {
        read(newsocket_fd, buffer, 255);
        fprintf(f, "%s ", buffer);
        words--;
    }

    printf("File received successfully...\n");
    printf("Closing connection...\n");

    close(newsocket_fd);
    close(server_fd);
    fclose(f);

    return 0;
}
