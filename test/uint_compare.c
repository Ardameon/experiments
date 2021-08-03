#include <stdio.h>
#include <stdint.h>

int is_less_then_zero(uint16_t arg)
{
    return arg < 0;
}

int main(int argc, char const *argv[])
{
    uint16_t val;

    val = 0;
    printf("%u is less then zero - %s\n", val, is_less_then_zero(val) ? "yes" : "no");

    val = 23;
    printf("%u is less then zero - %s\n", val, is_less_then_zero(val) ? "yes" : "no");

    val = -1;
    printf("%u is less then zero - %s\n", val, is_less_then_zero(val) ? "yes" : "no");

    val = -28;
    printf("%u is less then zero - %s\n", val, is_less_then_zero(val) ? "yes" : "no");

    int ival = -21;

    printf("%d is less then zero - %s\n", ival, is_less_then_zero(ival) ? "yes" : "no");

    return 0;
}