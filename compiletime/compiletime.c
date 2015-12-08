#include <stdio.h>
#include <linux/types.h>
#include <sys/types.h>

#ifndef ARR_SIZE
#define ARR_SIZE 5
#endif

struct st {
	char array[ARR_SIZE];
};

struct st_one {
	u_int8_t a:4,
			b:1,
			c:2,
			d:1;
};

int main(void)
{
	struct st str;
	struct st_one str1 = {0};

	printf("Arr size: %lu\n", sizeof(str.array));

	printf("a: %d\n"
		   "b: %d\n"
		   "c: %d\n"
		   "d: %d\n", str1.a, str1.b, str1.c, str1.d);

	int b = 255;

	str1.a = b;


	printf("a: %d\n"
		   "b: %d\n"
		   "c: %d\n"
		   "d: %d\n", str1.a, str1.b, str1.c, str1.d);


	return 0;
}
