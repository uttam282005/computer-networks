CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executables
TARGETS = server client

# Object files
SERVER_OBJS = server.o
CLIENT_OBJS = client.o

all: $(TARGETS)

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o server $(SERVER_OBJS)

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o client $(CLIENT_OBJS)

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c client.h
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o $(TARGETS)

.PHONY: all clean
