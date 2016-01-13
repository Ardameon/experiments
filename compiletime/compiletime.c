#include <stdio.h>
#include <linux/types.h>
#include <sys/types.h>
#include <assert.h>

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

int func_with_static()
{
	static int initial = 1;

	printf("%s: initial: %d\n", __func__, initial);

	if(initial) initial = 0;
}

int main(void)
{
	struct st str;
	struct st_one str1 = {0};
	int a, b, c;
	int i;

	printf("Arr size: %lu\n", sizeof(str.array));

	printf("a: %d\n"
		   "b: %d\n"
		   "c: %d\n"
		   "d: %d\n", str1.a, str1.b, str1.c, str1.d);

	int k = 255;

	str1.a = k;


	printf("a: %d\n"
		   "b: %d\n"
		   "c: %d\n"
		   "d: %d\n", str1.a, str1.b, str1.c, str1.d);

	
	assert(str1.d == 1);

	c = a=2,b=2;

	c = (a == 2),(a == 1);

	printf("val = %d\n", c);

	for(i = 0; i < 4; i++)
		func_with_static();

	return 0;
}
