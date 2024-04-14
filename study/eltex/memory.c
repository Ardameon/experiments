#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void foo(uint8_t *p);
void foo2(uint8_t p[][2]);

int main()
{
    int i;
    uint8_t  *u8ptr = malloc(1);
    uint16_t *u16ptr = malloc(2);
    uint32_t *u32ptr = malloc(4);

    uint8_t arr2[2][2] = { { 0x11, 0x22 }, { 0x33, 0x44 } };
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

    foo(arr);
    foo2(arr2);

    return 0;
}

void foo(uint8_t *p)
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
}

void foo2(uint8_t p[][2])
{
    printf("%-5s: &p:%p sizeof(p) = %zd sizeof(*p) = %zd\n", __func__, &p, sizeof(p), sizeof(*p));
}
