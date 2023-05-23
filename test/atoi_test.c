#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *str = "111*333*222";

    printf("atoi %s -> %d\n", str, atoi(str));

    return 0;
}
