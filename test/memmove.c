#include <stdio.h>
#include <stdint.h>
#include <string.h>


int main()
{
	char array[16] = {0};
	int array_i[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	char *start, *finish;
	int len = sizeof(array);
	char *c;
	int *i, *j, k;

	strcpy(array, "0123456789");


	start = &array[2];
	finish = &array[6];

	printf("'%s'\n", array);

	// memmove(start, finish, len - (int)(finish - start));
	// memcpy(start, finish, len - (int)(finish - start));

	c = &array[4];
	
	printf("c:'%c' c + 1:'%c' sizeof(array):%d shift:%d\n", *c, *(c + 1), sizeof(array), ((c + 1) - array));

	memcpy(c, c + 1, sizeof(array) - ((c + 1) - array));

	printf("'%s'\n", array);
	
	i = &array_i[4];

	printf("i:%d i + 1:%d sizeof(array_i):%d shift:%d\n", *i, *(i + 1), sizeof(array_i), ((i + 1) - array_i));
	
	memcpy(i, i + 1, sizeof(array_i) - ((i + 1) - array_i) * sizeof(*i));

	printf("'%s'\n", array);
	
	for (k = 0; k < sizeof(array_i) / sizeof(int); k++)
	{
		printf("%d", array_i[k]);
	}

	printf("\n");

	i = &array_i[3];
	j = &array_i[7];

	printf("pointers %p and %p difference is %d but shift is %d\n", j, i, (j - i) * sizeof(*i), j - i);

	return 0;
}
