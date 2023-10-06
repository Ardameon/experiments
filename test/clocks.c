#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t fib(uint32_t n)
{
	return n < 2 ? 1 : fib(n - 2) + fib(n - 1);
}

uint32_t fib_cycle(uint32_t n)
{
    uint32_t i = 2;
    uint32_t fib[2] = {1, 1};

    if (n < 2)
        goto _exit;

    for (i = 2; i <= n; i++)
    {
        fib[1] = fib[0] + (fib[0] = fib[1]);
    }

_exit:
    return fib[1];
}

int main(int argc, char **argv)
{
    int fib_num = 100;
    uint32_t fib_res = 0;
    clock_t clocks;

    if (argc > 1)
    {
        fib_num = atoi(argv[1]);
    }

    clocks  = clock();
    fib_res = fib(fib_num);
    clocks  = clock() - clocks;

    printf("Clocks for fib_cycle %d (%u):\n %ld (%lf sec)\n", fib_num, fib_res, clocks, (double)clocks / CLOCKS_PER_SEC);

    clocks  = clock();
    fib_res = fib_cycle(fib_num);
    clocks  = clock() - clocks;

    printf("Clocks for fib_cycle %d (%u):\n %ld (%lf sec)\n", fib_num, fib_res, clocks, (double)clocks / CLOCKS_PER_SEC);
}
