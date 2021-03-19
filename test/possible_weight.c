#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int weight_arr [] = {
	77,
	100,
	25
};

int get_random(int *arr, int size)
{
	int num, total_value, i;

	for (i = 0, total_value = 0; i < size; i++)
	{
		total_value += arr[i];
	}

	num = rand() % total_value;

	for (i = 0, total_value = 0; i < size; i++)
	{
		total_value += arr[i];
		if (num < total_value) break;
	}

	return i;
}

int main()
{
	int result[3] = {0};
	int i, idx;

	srand(time(0));

	for (i = 0; i < 10000000; i++)
	{
		result[get_random(weight_arr, 3)]++;
	}

	for (i = 0; i < sizeof(result)/sizeof(int); i++)
	{
		printf("res[%d]: %d\n", i, result[i]);
	}

	return 0;
}