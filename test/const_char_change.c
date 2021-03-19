#include <stdio.h>

const char glob;

char *func()
{
	return "STR";
}

void func_1()
{
	char *p = func();
	static char stat;

	printf("infunc: %p %p %p\n", func(), &stat, &glob);
}

int main()
{
	func_1();

	printf("outfunc: %p\n", func());

	return 0;
}
