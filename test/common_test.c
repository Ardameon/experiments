#include <stdio.h>

static inline char *move(char *p)
{
	return p += 3;
}

#define MOVE(p) \
do {        \
	p += 2; \
	p += 1; \
} while(0)

int main(int argc, char const *argv[])
{
	char *p = "0123456789";
	char str[12];
	int cnt;
	char str1[] = "123";
	char *p1;

	printf("before: '%c'\n", *p);

	MOVE(p);

	printf("after: '%c'\n", *p);

	cnt = sprintf(str, "1234");

	printf("cnt: %d str[cnt]: '%d'\n", cnt, str[cnt]);

	p1 = str1;

	*p1++ = '7';
	*p1++ = '8';

	printf("str1: '%s'", str1);

	return 0;
}
