#include <stdio.h>

int main()
{
	int a = -6;
	printf("a      = %d\n", a);
	printf("a << 1 = %d\n", a << 1);
	printf("a >> 1 = %d\n", a >> 1);
	printf("a << 2 = %d\n", a << 2);
	printf("a >> 2 = %d\n", a >> 2);

	printf("a: %#x\n", a);

	return 0;
}
