#include <stdio.h>
#include <stdlib.h>

enum en {
	E_1,
	E_2
} state;


int fib(int n)
{
	return n < 2 ? 1 : fib(n - 2) + fib(n - 1);
}

int main(int argc, char **argv)
{
	int number = atoi(argv[1]);

	printf("%d\n", fib(number));

	state = E_2;

	printf("%d\n", state);

	return 0;
}
