#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

int fun()
{
	printf("AAAA\n");

	return 1000;
}

int main()
{
	int i = 0;
	int a = 0;

	for (i = 0; i < fun(); i++)
	{
		printf("a = %d\n", a++);
	}

	return 0;
}
