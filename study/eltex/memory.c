#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    uint8_t a;
    uint16_t b;
    uint32_t c;
} __attribute__ ((packed)) struct_t;

void foo(uint8_t *p);
void foo2(uint8_t p[][5]);

int gvar;

int main(void)
{
    static int svar;
    int i;
    uint8_t  *u8ptr = malloc(1);
    uint16_t *u16ptr = malloc(2);
    uint32_t *u32ptr = malloc(4);
    void (*fptr1)(uint8_t *) = &foo;
    struct_t *sptr;

    uint8_t arr2[2][5] = { { 0x66, 0x77, 0x88, 0x99, 0xAA }, { 0x11, 0x22, 0x33, 0x44, 0x55 } };
    uint8_t arr[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC };

    printf("Sizeof:\n");
    printf("sizeof(u8ptr)  = %zd sizeof(*u8ptr)  = %zd\n",   sizeof(u8ptr),  sizeof(*u8ptr));
    printf("sizeof(u16ptr) = %zd sizeof(*u16ptr) = %zd\n", sizeof(u16ptr), sizeof(*u16ptr));
    printf("sizeof(u32ptr) = %zd sizeof(*u32ptr) = %zd\n", sizeof(u32ptr), sizeof(*u32ptr));

    printf("Array:\n");

    printf("sizeof(arr) = %zd\n", sizeof(arr));

    printf("u8ptr:   %p\n", u8ptr);
    printf("u16ptr:  %p\n", u16ptr);
    printf("u32ptr:  %p\n", u32ptr);
    printf("&u8ptr:  %p\n", &u8ptr);
    printf("&u16ptr: %p\n", &u16ptr);
    printf("&u32ptr: %p\n", &u32ptr);
    printf("arr:     %p\n", arr);
    printf("arr2:    %p\n", arr2);
    printf("fptr1:   %p\n", fptr1);
    printf("foo:     %p\n", foo);
    printf("foo2:    %p\n", foo2);
    printf("main:    %p\n", main);
    printf("gvar:    %p\n", &gvar);
    printf("svar:    %p\n", &svar);
    printf("\"1\":     %p\n", "1");

    for (i = 0; i < sizeof(arr); i++)
    {
        printf("\t arr[%2d](%p): %#x\n", i, (arr + i), *(arr + i));
    }

    u16ptr = arr;

    for (i = 0; i < sizeof(arr) / sizeof(*u16ptr); i++)
    {
        printf("\t u16ptr[%d](%p): %#x\n", i, (u16ptr + i), *(u16ptr + i));
    }

    u32ptr = arr;

    for (i = 0; i < sizeof(arr) / sizeof(*u32ptr); i++)
    {
        printf("\t u32ptr[%d](%p): %#x\n", i, (u32ptr + i), *(u32ptr + i));
    }

    u8ptr = arr2;

    for (i = 0; i < sizeof(*arr2) * 2; i++)
    {
        printf("\t u8ptr[%d](%p): %#x\n", i, (u8ptr + i), *(u8ptr + i));
    }

    sptr = arr2;

    printf("Struct:\n");
    printf("a(%p): %#x\n", &sptr->a, sptr->a);
    printf("b(%p): %#x\n", &sptr->b, sptr->b);
    printf("c(%p): %#x\n", &sptr->c, sptr->c);

    foo(arr);
    fptr1(arr);
    foo2(arr2);

    return 0;
}

void foo(uint8_t *p)
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
}

void foo2(uint8_t p[][5])
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
    printf("\"1\":   %p\n", "1");
}
