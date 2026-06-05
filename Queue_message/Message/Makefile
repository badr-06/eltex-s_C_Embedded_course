CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lncurses

all: server client

server: server.c library.h
	$(CC) $(CFLAGS) server.c library.c -o server

client: client.c library.h
	$(CC) $(CFLAGS) client.c library.c -o client $(LIBS)

clean:
	rm -f server client

run-server:
	./server

run-client:
	./client