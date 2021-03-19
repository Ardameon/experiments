#include <stdio.h>


int main(int argc, char const *argv[])
{
	int arr[10];
	int *p = arr;
	int res;
	int i;

	for (i = 0; i < 10; i++)
	{
		*p++ = i;
		printf("arr[%d] = %d\n", i, arr[i]);
	}

	p = arr;
	*arr = 0;
	res = *p;
	printf("none p = %p (res:%d)(arr:%d)\n", p, res, *arr);
	
	p = arr;
	*arr = 0;
    res = *p++;
	printf("*p++ p = %p (res:%d)(arr:%d)\n", p, res, *arr);

	p = arr;
	*arr = 0;
    res = (*p)++;
	printf("(*p)++ p = %p (res:%d)(arr:%d)\n", p, res, *arr);

	p = arr;
	*arr = 0;
    res = *(p++);
	printf("*(p++) p = %p (res:%d)(arr:%d)\n", p, res, *arr);

	return 0;
}
