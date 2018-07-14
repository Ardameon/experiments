#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#define  IP4_MAX_LEN 15
#define  PORT_NUM 23232
#define  POLL_TIMEOUT 5000

int gl_sock;
int loop = 1;

void sigint_handler(int sig) {
    printf("SIGINT handler: close socket.\n");
    loop = 0;
    shutdown(gl_sock, SHUT_RDWR);
    close(gl_sock);
}

static const char *getLocalIP(const char *iface)
{
    static char ip_addr_str[IP4_MAX_LEN + 1];
    struct ifreq ifr;

    strcpy(ifr.ifr_name, iface);

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    ioctl(s, SIOCGIFADDR, &ifr);
    close(s);

    struct sockaddr_in *sa = (struct sockaddr_in *)&ifr.ifr_addr;

    strcpy(ip_addr_str, inet_ntoa(sa->sin_addr));

    return ip_addr_str;
}

int main(int argc, char **argv)
{
    int sock = -1;
    int client_sock = -1;
    int ret_status = 0;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
    int reuse = 1;
    char buf[1024] = {0};
    int len, res;
    char local_ip[IP4_MAX_LEN];
    unsigned local_port = PORT_NUM;
    struct pollfd fds[1];
    int is_sender = 0;
    int tick;
    int wait_time = 5;
//    char test_msg[] = "0000111122223333";
    uint8_t test_sorm_msg[] = {0xCC, 0x00, 0x01, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36};

    if(argc < 2)
    {
        printf("Not enough arguments: [PORT] [IS_SENDER]\n");
        goto _exit;
    }

    local_port = strtoul(argv[1], NULL, 10);

    if(argc == 3)
    {
        is_sender = strtoul(argv[2], NULL, 10);
    }

    do
    {

    strcpy(local_ip, getLocalIP("eth0"));
    printf("Local IP is '%s:%d'\n", local_ip, local_port);

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(local_ip);
    local_addr.sin_port = htons(local_port);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

//    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &set_opt_flag, sizeof(set_opt_flag));

    signal(SIGINT, &sigint_handler);

    gl_sock = sock;

    printf("Client socket created. fd: %d\n", sock);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }


    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    printf("Server socket binded to '%s:%u'\n", inet_ntoa(local_addr.sin_addr),
           local_port);

    if (listen(sock, 10) < 0) {
        perror("listen() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    if ((client_sock = accept(sock, (struct sockaddr *) &remote_addr,
                              &remote_addr_len)) != -1) {
        printf("New connection. Client: fd: %d '%s:%u'\n",  client_sock,
               inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
    } else {
        perror("accept() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

    fds[0].fd = client_sock;
    fds[0].events = POLLIN;

    tick = 0;

    while(1)
    {
        if(is_sender && tick >= wait_time)
        {
            if ((len = send(client_sock, test_sorm_msg, sizeof(test_sorm_msg), 0)) != -1) {
                printf("Data sent to client len=%d buf='%s'\n",
                       len, test_sorm_msg);

            } else {
                perror("send() failed");
//                ret_status = EXIT_FAILURE;
//                break;
            }

            tick = 0;
        }

        tick++;

        res = poll(fds, 1, POLL_TIMEOUT);

        if(res < 0) {
            perror("poll()");
            ret_status = EXIT_FAILURE;
            break;
        }

        printf("revents: %04x%s\n", fds[0].revents, !res ? " (timeout)" : "");

        if(res == 0 || !(fds[0].revents & POLLIN)) {
            printf("No input data on socket\n");

            if ((len = send(client_sock, buf, 1, MSG_PEEK | MSG_DONTWAIT)) != -1) {
                if(len == 0)
                {
                    printf("send len is 0\n");
                } else {
                    printf("Data sent to client len=%d buf='%s'\n",
                           len, buf);
                }

            } else {
                perror("send() failed");
    //            ret_status = EXIT_FAILURE;
    //            break;
            }

            continue;
        }

        if ((len = recv(client_sock, buf, sizeof(buf), 0)) != -1) {
            if(len == 0)
            {
                printf("Connection closed by remote side\n");
                break;
            } else {
                printf("Data received form client len=%d buf='%s'\n",
                       len, buf);
            }

        } else {
            perror("recv() failed");
//            ret_status = EXIT_FAILURE;
//            break;
        }
    }

_exit:
    if(sock != -1) close(sock);

    } while(loop);


    return ret_status;
}
