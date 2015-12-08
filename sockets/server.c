#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#define  SRV_SOCK_PATH "server_socket"

int gl_sock;

void sigint_handler(int sig) {
    printf("SIGINT handler: close socket.\n");
    shutdown(gl_sock, SHUT_RDWR);
    close(gl_sock);
    unlink(SRV_SOCK_PATH);
}

int main()
{
    int sock = -1;
    int client_sock = -1;
    int ret_status = 0;
    struct sockaddr_un srv_addr;
    struct sockaddr_un client_addr;
    int client_addr_len;
    int reuse = 1;
    char buf[1024] = {0};
    int len;

    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, SRV_SOCK_PATH);

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    signal(SIGINT, &sigint_handler);

    gl_sock = sock;

    printf("Server socket created. fd: %d\n", sock);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    if (bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
        perror("bind() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    printf("Server socket binded to '%s'\n", srv_addr.sun_path);

    if (listen(sock, 10) < 0) {
        perror("listen() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    if ((client_sock = accept(sock, (struct sockaddr *) &client_addr,
                              (socklen_t *)&client_addr_len)) != -1) {
        printf("New connection. Client: fd: %d '%s'\n",  client_sock,
               client_addr.sun_path);
    } else {
        perror("accept() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    if ((len = recv(client_sock, buf, sizeof(buf), 0)) != -1) {
        printf("Data received:  <<<<< len=%d buf='%s'\n", len, buf);
    } else {
        perror("recv() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    strcpy(buf, "Message received. Thank you!");

    if ((len = send(client_sock, buf, strlen(buf), 0) != -1)) {
        printf("Data sent:  >>>>> len=%d buf='%s'\n", len, buf);
    } else {
        perror("send() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    sleep(30);

_exit:
    if (sock > -1) {
        shutdown(sock, SHUT_RDWR);
        close(sock);
        unlink(SRV_SOCK_PATH);
    }

    return ret_status;
}
