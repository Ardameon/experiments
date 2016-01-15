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

#define  IP4_MAX_LEN 15
#define  PORT_NUM 23232

int gl_sock;

void sigint_handler(int sig) {
    printf("SIGINT handler: close socket.\n");
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

int main()
{
    int sock = -1;
    int ret_status = 0;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    int reuse = 1;
    char buf[1024] = {0};
    int len, res, rem_addr_len;
    char local_ip[IP4_MAX_LEN];
    unsigned local_port = PORT_NUM;
    struct pollfd fds[1];

    strcpy(local_ip, getLocalIP("eth0"));
    printf("Local IP is '%s:%d'\n", local_ip, PORT_NUM);

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(local_ip);
    local_addr.sin_port = htons(local_port);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket() failed");
        ret_status = EXIT_FAILURE;
        goto _exit;
    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

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

    fds[0].fd = sock;
    fds[0].events = POLLIN;

    while(1)
    {
        res = poll(fds, 1, 1000);

        if(res < 0) {
            perror("poll()");
            ret_status = EXIT_FAILURE;
            break;
        }

        if(res == 0 || !(fds[0].revents & POLLIN)) {
            printf("No input data on socket\n");
            continue;
        }

        if ((len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&remote_addr,
                            (socklen_t *)&rem_addr_len)) != -1) {
            printf("Data received form %s:%d len=%d buf='%s'\n",
                   inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port),
                   len, buf);
        } else {
            perror("recvfrom() failed");
            ret_status = EXIT_FAILURE;
            break;
        }
    }

_exit:
    if(sock != -1) close(sock);

    return ret_status;
}
