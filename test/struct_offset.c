#include <stdio.h>
#include <stdint.h>

typedef void (*print_cb)(void);

typedef struct
{
    int a;
    int b;
    int c;
    int d[10];
    print_cb cb;
} test_t;

void my_print(void)
{
    printf("%s: start", __func__);
}

int main(void)
{
    test_t ttt;
    uint8_t *ptr = (uint8_t *)&ttt;
    unsigned long offset = (unsigned long)(&((test_t *)NULL)->d);

    printf("start:%p field:%p\n", ptr, ptr + offset);
    printf("sizeof:%lu\n", sizeof((test_t *)ptr + offset));

    ttt.cb = &my_print;
    ttt.cb = my_print;

    return 0;
}
