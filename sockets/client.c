#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#define  CLT_SOCK_PATH "client_socket"
#define  SRV_SOCK_PATH "server_socket"

int gl_sock;

void sigint_handler(int sig) {
    printf("SIGINT handler: close socket.\n");
    shutdown(gl_sock, SHUT_RDWR);
    close(gl_sock);
    unlink(CLT_SOCK_PATH);
}

int main()
{
    int sock = -1;
    int ret_status = 0;
    struct sockaddr_un client_addr;
    struct sockaddr_un srv_addr;
    int reuse = 1;
    char buf[1024] = {0};
    int len;

    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, CLT_SOCK_PATH);

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    signal(SIGINT, &sigint_handler);

    gl_sock = sock;

    printf("Client socket created. fd: %d\n", sock);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    if (bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    printf("Client socket binded to '%s'\n", client_addr.sun_path);

    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, SRV_SOCK_PATH);

    if (connect(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
        perror("connect() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    strcpy(buf, "Test message from client.");

    if ((len = send(sock, buf, strlen(buf), 0) != -1)) {
        printf("Data sent  >>>>>: len=%d buf='%s'\n", len, buf);
    } else {
        perror("send() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    if ((len = recv(sock, buf, sizeof(buf), 0)) != -1) {
        printf("Data received  <<<<<: len=%d buf='%s'\n", len, buf);
    } else {
        perror("recv() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

_exit:
    if (sock > -1) {
        shutdown(sock, SHUT_RDWR);
        close(sock);
        unlink(CLT_SOCK_PATH);
    }

    return ret_status;
}
