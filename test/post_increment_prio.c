#include <stdio.h>

int func1(int i)
{
	printf("%s: i = %d\n", __func__, i);
	return 0;
}

int func2(int i)
{
	printf("%s: i = %d\n", __func__, i);
	return 1;
}

int main(int argc, char const *argv[])
{
	int i = 2;

	while (1)
	{
		func1(i);
		if (!i--)
		{
			func2(i);
			break;
		}

	}

	return 0;
}
