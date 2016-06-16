#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/ipv6.h>

#include "netdevice.h"

int main(int argc, char **argv)
{
    char buf[INET6_ADDRSTRLEN];
    int i;

    if(argc < 3)
    {
        printf("Not anough arguments\n");
        return -1;
    }

    struct in6_addr addr;

    printf("Argument: '%s'\n", argv[2]);

    inet_pton(AF_INET6, argv[2], &addr);

    printf("Bytes 8: ");
    for (i = 0; i < 15; i++)
        printf("%02x:", addr.__in6_u.__u6_addr8[i]);
    printf("%02x\n", addr.__in6_u.__u6_addr8[i]);

    printf("Bytes 16: ");
    for (i = 0; i < 7; i++)
        printf("%04x:", addr.__in6_u.__u6_addr16[i]);
    printf("%04x\n", addr.__in6_u.__u6_addr16[i]);

    inet_ntop(AF_INET6, &addr, buf, INET6_ADDRSTRLEN);

    printf("Converted back: '%s'\n", buf);

    if_set_ip6(argv[1], argv[2], 32);
    if_set_flag(argv[1], NETDEV_UP_FLAG_SET);

    return 0;
}
