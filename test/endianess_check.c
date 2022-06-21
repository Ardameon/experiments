#include <stdio.h>
#include <stdint.h>

struct st {
    union 
    {
        uint16_t val;
        uint8_t arr[2];
    } u;
};

int main(int argc, char const *argv[])
{
    struct st check;

    check.u.val = 0x1122;

    printf("val: %#x  arr = {%#hx, %#hx}\n", check.u.val, check.u.arr[0], check.u.arr[1]);

    return 0;
}