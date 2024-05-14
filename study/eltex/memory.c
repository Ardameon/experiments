#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MACRO_STR "1"

typedef struct
{
    uint8_t a;
    uint16_t b;
    uint32_t c;
} __attribute__ ((packed)) struct_t;

void foo(uint8_t *p);
void foo2(uint8_t p[][5]);
void foo3(uint8_t **p);
void foo4(void *ptr);

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

    uint8_t arr[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC };
    int arr_int[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC };

    uint8_t arr2[2][5] = { { 0x66, 0x77, 0x88, 0x99, 0xAA }, { 0x11, 0x22, 0x33, 0x44, 0x55 } };

    printf("Sizeof:\n");
    printf("sizeof(u8ptr)  = %zd sizeof(*u8ptr)  = %zd\n", sizeof(u8ptr),  sizeof(*u8ptr));
    printf("sizeof(u16ptr) = %zd sizeof(*u16ptr) = %zd\n", sizeof(u16ptr), sizeof(*u16ptr));
    printf("sizeof(u32ptr) = %zd sizeof(*u32ptr) = %zd\n", sizeof(u32ptr), sizeof(*u32ptr));

    printf("Array:\n");

    printf("sizeof(arr)     = %zd\n", sizeof(arr));
    printf("sizeof(arr_int) = %zd\n", sizeof(arr_int));

    printf("    arr[1]: %d\n", arr[1]);
    printf("*(arr + 1): %d\n", *(arr + 1));
    printf("    1[arr]: %d\n", 1[arr]);

    printf("u8ptr:   %p\n", u8ptr);
    printf("u16ptr:  %p\n", u16ptr);
    printf("u32ptr:  %p\n", u32ptr);
    printf("&u8ptr:  %p\n", &u8ptr);
    printf("&u16ptr: %p\n", &u16ptr);
    printf("&u32ptr: %p\n", &u32ptr);
    printf("arr:     %p\n", arr);
    printf("arr2:    %p\n", arr2);
    printf("arr_int: %p\n", arr_int);
    printf("fptr1:   %p\n", fptr1);
    printf("&fptr1:  %p\n", &fptr1);
    printf("foo:     %p\n", foo);
    printf("foo2:    %p\n", foo2);
    printf("foo3:    %p\n", foo3);
    printf("main:    %p\n", main);
    printf("gvar:    %p\n", &gvar);
    printf("svar:    %p\n", &svar);
    printf("\"1\":     %p\n", "1");

    char *str_lit_ptr = MACRO_STR;
    foo4(str_lit_ptr);

    for (i = 0; i < sizeof(arr); i++)
    {
        printf("\t arr[%2d](%p): %#x\n", i, (arr + i), *(arr + i));
    }

    u16ptr = (uint16_t *)arr;

    for (i = 0; i < sizeof(arr) / sizeof(*u16ptr); i++)
    {
        printf("\t u16ptr[%d](%p): %#x\n", i, (u16ptr + i), *(u16ptr + i));
    }

    u32ptr = (uint32_t *)arr;

    for (i = 0; i < sizeof(arr) / sizeof(*u32ptr); i++)
    {
        printf("\t u32ptr[%d](%p): %#x\n", i, (u32ptr + i), *(u32ptr + i));
    }

    u8ptr = (uint8_t *)arr2;

    for (i = 0; i < sizeof(*arr2) * 2; i++)
    {
        printf("\t u8ptr[%d](%p): %#x\n", i, (u8ptr + i), *(u8ptr + i));
    }

    foo(arr);
    fptr1(arr);

    foo2(arr2);
    foo3((uint8_t **)arr2);

    sptr = arr2;

    printf("Struct:\n");
    printf("a(%p): %#x\n", &sptr->a, sptr->a);
    printf("b(%p): %#x\n", &sptr->b, sptr->b);
    printf("c(%p): %#x\n", &sptr->c, sptr->c);

    return 0;
}

void foo(uint8_t *p)
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
}

void foo2(uint8_t p[][5])
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
    printf("%-5s: p[0]:%p | *(p + 0):%p\n", __func__, p[0], *(p + 0));
    printf("%-5s: p[1]:%p | *(p + 1):%p\n", __func__, p[1], *(p + 1));
    //printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd p[1][1] = %#x\n", __func__, &p, sizeof(p), sizeof(*p), p[1][1]);
}

void foo3(uint8_t **p)
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
    printf("%-5s: p[0]:%p | *(p + 0):%p\n", __func__, p[0], *(p + 0));
    printf("%-5s: p[1]:%p | *(p + 1):%p\n", __func__, p[1], *(p + 1));
    //printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd p[1][1] = %#x\n", __func__, &p, sizeof(p), sizeof(*p), p[1][1]);
}

void foo4(void *ptr)
{
    printf("ptr:   %p\n", ptr);
    printf("\"1\":   %p\n", "1");
}
