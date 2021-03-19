#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void bubble(char **arr, int size)
{
	int i, j;
	char *swap;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (strcmp(arr[j], arr[j + 1]) > 0)
			{
				swap = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = swap;
			}
		}
	}
}

int compare(const void *a, const void *b)
{
	return strcmp(*(char **)a, *(char **)b);
}

// sz1: size of array1, sz2: size of array2, lg: size of the returned array
char** inArray(char* array1[], int sz1, char* array2[], int sz2, int* lg) 
{
  	char **r = malloc(sizeof(char *) * sz1);
  	int i, j, k;
  	int *found = calloc(sizeof(int), sz1);

  	for (i = 0, k = 0; i < sz2; i++)
  	{
  		for (j = 0; j < sz1; j++)
  		{
			printf("Check '%s' and '%s'\n", array2[i], array1[j]);

  			if (!found[j] && strstr(array2[i], array1[j]))
  			{
  				r[k++] = array1[j];
  				found[j] = 1;

  				printf("'%s' is substring '%s'\n", r[k - 1], array2[i]);
  				// break;
  			}
  		}
  	}

	for (i = 0; i < k; i++)
	{
		printf("%s ", r[i]);
	}
	printf("\n");

  	qsort(r, k, sizeof(*r), compare);

  	// bubble(r, k);
	
	for (i = 0; i < k; i++)
	{
		printf("%s ", r[i]);
	}
	printf("\n");

  	free(found);

  	*lg = k;
  	return r;
}

int main()
{
	char *a1[] = {"cod", "code", "wars", "ewar", "pillow", "bed", "phht"};
	char *a2[] = {"lively", "alive", "harp", "sharp", "armstrong", "codewars", "cod", "code"};
	int len, i;

	char **p = inArray(a1, 7, a2, 8, &len);

	for (i = 0; i < len; i++)
	{
		printf("%s ", p[i]);
	}

	printf("\n");

	return 0;
}
