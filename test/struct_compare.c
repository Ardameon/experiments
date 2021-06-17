#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct strc {
    int a;
    int b;
    uint16_t c;
    uint8_t d;
} my_struct;

int main(int argc, char const *argv[])
{
    my_struct st0, st1, st2;

    st0.a = 1;
    st0.b = -1;
    st0.c = 2;
    st0.d = 233;

    st1.a = 1;
    st1.b = -1;
    st1.c = 2;
    st1.d = 233;

    st1 = st0;

    st2.a = 1;
    st2.b = 1;
    st2.c = 2;
    st2.d = 233;

    // printf("st0 == st1 - %s\n", (st0 == st1) ? "true" : "false"); - COMPILE ERROR!!!
    // printf("st0 == st2 - %s\n", (st0 == st2) ? "true" : "false"); - COMPILE ERROR!!!

    printf("st0 == st1 - %s\n", !memcmp(&st0, &st1, sizeof(st0)) ? "true" : "false");
    printf("st0 == st2 - %s\n", !memcmp(&st0, &st2, sizeof(st1)) ? "true" : "false");

    return 0;
}