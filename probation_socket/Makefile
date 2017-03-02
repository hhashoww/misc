CC = gcc
CFLAGS = -g -Wall

all:
	$(CC) $(CFLAGS) -lpthread server.c my_configure.h -o server
	$(CC) $(CFLAGS) client.c my_configure.h -o client

clean:
	rm -f server
	rm -f client
