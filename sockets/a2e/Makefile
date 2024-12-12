all: server client

server:
	gcc -g -Wall -pedantic -Werror a2e.c a2e_msg.c server.c a2e_client.c a2e_server.c a2e_dbg.c a2e_common.c -o server

client:
	gcc -g -Wall -pedantic -Werror a2e.c a2e_msg.c client.c a2e_client.c a2e_server.c a2e_dbg.c a2e_common.c -o client

.PHONY: all client server
