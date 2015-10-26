#include <stdio.h>
#include <linux/types.h>

#ifndef ARR_SIZE
#define ARR_SIZE 5
#endif

struct st {
	char array[ARR_SIZE];
};

int main(void)
{
	struct st str;

	printf("Arr size: %lu\n", sizeof(str.array));

	return 0;
}
