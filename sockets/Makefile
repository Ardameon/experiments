CC = gcc
CFLAGS = -Wall -Werror
DEBUG = -g

all: server client

server: server.o
	$(CC) -o $@ $<

server.o: server.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

client: client.o
	$(CC) -o $@ $<

client.o: client.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<
