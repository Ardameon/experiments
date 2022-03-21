#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    char c = 0xF0;

    printf("c = %d\n", c);
    printf("c << 1 = %d\n", c << 1);
    printf("c >> 1 = %d\n", c >> 1);

	c = 7;

	printf("c >> 1 = %d\n", c >> 1);

	c = 10;

	printf("~c = %d\n", ~c);

    return 0;
}
