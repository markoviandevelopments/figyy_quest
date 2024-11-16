# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread

# Libraries for the client
CLIENT_LIBS = -lraylib -lm -pthread

# Source files
SRCS = server.c client.c

# Object files
OBJS = $(SRCS:.c=.o)

# Dependency files
DEPS = $(SRCS:.c=.d)

# Targets
all: server client

# Server target
server: server.o
	$(CC) $(CFLAGS) server.o -o server

# Client target
client: client.o
	$(CC) $(CFLAGS) client.o -o client $(CLIENT_LIBS)

# Rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate dependency files
%.d: %.c
	$(CC) $(CFLAGS) -MM $< -MF $@

# Include dependency files
-include $(DEPS)

# Clean target
clean:
	rm -f server client $(OBJS) $(DEPS)

.PHONY: all clean
