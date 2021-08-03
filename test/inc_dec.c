#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char const *argv[])
{
    int i = 2;
    int j = 3;
    int a;

    a = ++j + 4 + ++i + j++;

    printf("a = %d i = %d j = %d\n", a, i, j);

    int b = 7;
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint64_t c;
    int *p = &b;
    c = (uint64_t)p;

    printf("b = %d (size:%ld) p = %p (size:%ld)\n", b, sizeof(b), p, sizeof(p));
    printf("c = %#lx\n", c);

    printf("*c = %d\n", *(int *)c);

    printf("\n===============================================\n\n");
    printf("arr[4] = %d\n", arr[4]);
    printf("arr = %p\n", arr);
    printf("*arr = %d\n", *arr);

    int *p_ar = arr;
    char *p_ch = arr;

    printf("*p_ar = %d\n", p_ar[3]);
    printf("*p_ar = %d (%p)\n", *(p_ar + 3), p_ar + 3);

    printf("p_ch = %p\n", p_ch);
    printf("p_ch + 3 = %p\n", p_ch + 3);

    uint32_t arr1[2] = {0, 1};
    uint32_t arr2[2] = {2, 3};

    uint8_t *p_u;
    p_u = (uint8_t *)arr1;

    for (i = 0; i < 4; i++)
    {
        printf("p_u[%i]: %u\n", i, p_u[i]);
    }

    printf("arr1: %p arr2: %p\n", arr1, arr2);

    printf("arr1[3]: %d\n", arr1[3]);

    uint8_t ip_addr[4] = {192, 168, 76, 0};
    uint32_t *p_ip = ip_addr;

    struct in_addr addr;

    addr.s_addr = *p_ip;

    printf("ip_addr == '%s'\n", inet_ntoa(addr));

    if (TracesOn())
    {
        int bc;

        printf();
    }



    return 0;
}