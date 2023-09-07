#include <stdio.h>

int main()
{
    char a = 0xFF;

    printf("char '%d' == 0xFF is %s\n", a, a == 0xFF ? "TRUE" : "FALSE");
    return 0;
}
