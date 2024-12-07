all: server client

server:
	gcc -g -Wall -Werror a2e.c server.c a2e_client.c a2e_server.c a2e_dbg.c a2e_common.c -o server

client:
	gcc -g -Wall -Werror a2e.c client.c a2e_client.c a2e_server.c a2e_dbg.c a2e_common.c -o client

.PHONY: all client server
