# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread

# Libraries for the client
CLIENT_LIBS = -lraylib -lm -pthread

# Targets
all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client $(CLIENT_LIBS)

clean:
	rm -f server client

.PHONY: all clean
