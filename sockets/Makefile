CC = gcc
CFLAGS = -Wall -Werror
DEBUG = -g

all: server client client_dgram server_dgram server_tcp client_tcp

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

server_tcp: server_tcp.o
	$(CC) -o $@ $<

server_tcp.o: server_tcp.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

client_tcp: client_tcp.o
	$(CC) -o $@ $<

client_tcp.o: client_tcp.c
	$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

clean_bin:
	rm server client client_dgram server_dgram client_tcp server_tcp

clean: clean_bin
	rm *.o

