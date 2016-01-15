CC = gcc
CFLAGS = -Wall -Werror
DEBUG = -g

all: server client client_dgram server_dgram

server: server.o
	$(CC) -o $@ $<

server.o: server.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

client: client.o
	$(CC) -o $@ $<

client.o: client.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

client_dgram: client_dgram.o
	$(CC) -o $@ $<

client_dgram.o: client_dgram.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

server_dgram: server_dgram.o
	$(CC) -o $@ $<

server_dgram.o: server_dgram.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

cleanall: clean
	rm server client client_dgram

clean:
	rm *.o

