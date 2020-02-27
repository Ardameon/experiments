#include <stdlib.h>
#include <stdio.h>
#include "increase.h"



int main(int argc, char **argv)
{
	int num;

	num = 4;

	printf("input:%d result:%d\n", num, increase(num));

	num = 5;

	printf("input:%d result:%d\n", num, increase(num));

	return 0;
}

