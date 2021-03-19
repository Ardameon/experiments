#include <stdio.h>
#include <stdint.h>

struct st
{
	uint8_t a:1,
	        b:2,
	        :0;
};

int main()
{
	struct st tmp;

	tmp.a = 2;
	tmp.b = 7;

	printf("\na:%d b:%d\n", tmp.a, tmp.b);

	tmp.a = 5;
	tmp.b = 6;

	printf("a:%d b:%d\n", tmp.a, tmp.b);

	tmp.a = 10;
	tmp.b = 22;

	printf("a:%d b:%d\n", tmp.a, tmp.b);

	return 0;
}