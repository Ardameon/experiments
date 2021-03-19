#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char c = 'a';

char *func()
{
	char *p = "test_string1";
	char str[20];
	char *s;
	const char str1[20] = "test_const";
	int a;
	char *pp = malloc(1);
	static char b = 'b';

	strcpy(str, p);

	printf("p:%p str:%p a:%p pp:%p c:%p b:%p\n", p, str, &a, pp, &c, &b);

	s = &str;

	printf("'%p' '%p'\n", str, s);

	return p;
}

typedef int (*my_func_t)(int a, int b);
typedef int my_int_t;

int main()
{
	my_func_t func_arr[10];
	void (*func_arr1[10]) (char a);
	int *int_arr[10];
	my_int_t i;

	printf("p is: %s\n", func());

	memset(func_arr, 0, sizeof(func_arr));
	memset(func_arr1, 0, sizeof(func_arr1));
	memset(int_arr, 0, sizeof(int_arr));

	printf("szieof func_arr: %d\n", sizeof(func_arr) / sizeof(*func_arr));
	printf("szieof func_arr1: %d\n", sizeof(func_arr1) / sizeof(*func_arr1));
	printf("szieof int_arr: %d\n", sizeof(int_arr) / sizeof(*int_arr));

	for (i = 0; i < sizeof(func_arr) / sizeof(*func_arr); i++)
	{
		printf("func_arr[%d]: %p\n", i, func_arr[i]);
	}

	for (i = 0; i < sizeof(func_arr1) / sizeof(*func_arr1); i++)
	{
		printf("func_arr1[%d]: %p\n", i, func_arr1[i]);
	}

	for (i = 0; i < sizeof(int_arr) / sizeof(*int_arr); i++)
	{
		printf("int_arr[%d]: %p\n", i, int_arr[i]);
	}

	return 0;
}
