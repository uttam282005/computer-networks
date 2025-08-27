#include "client.h"
#include <ctype.h>
#include <stdio.h>
#include <strings.h>

enum Status {
    SUCCESS = 0,
    FAILURE = -1,
};

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock_fd;
    char buffer[255];
    enum Status status;

    if (argc < 3) {
        error("Need server port number and Ip address...");
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    char *ipaddr = argv[1];
    int portno = atoi(argv[2]);

    struct sockaddr_in server_addr;
    struct hostent *server;

    server = gethostbyname(ipaddr);
    if (server == NULL) {
        error("No such server...");
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    bcopy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if ((status = connect(sock_fd, (struct sockaddr *)&server_addr,
                          sizeof(server_addr))) == FAILURE) {
        error("Error connecting to server...");
    }

    printf("Connection established...\n");

    FILE *f = fopen("glad.txt", "r");
    int words = 0;
    char c;

    while ((c = getc(f)) != EOF) {
        if (isspace(c) || c == '\t') {
            words++;
        }
    }

    if ((status = write(sock_fd, &words, sizeof(int))) == FAILURE) {
        error("Error writing to socket...");
    }

    rewind(f);
    bzero(buffer, 255);

    while(words) {
        fscanf(f, "%s", buffer);
        fprintf(f, "%s ", buffer);
        write(sock_fd, buffer, 255);
        words--;
    }

    printf("File send successfully...\n");
    printf("Closing connection...\n");

    close(sock_fd);
    fclose(f);
    return 0;
}
