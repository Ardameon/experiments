#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *new_1 = NULL, *new_2 = NULL, *new_3 = NULL;
    char *p = "string";
    char *p_empty = "";

    new_1 = strdup(p);
    new_2 = strdup(p_empty);
    // new_3 = strdup(NULL);

    printf("n1:%p('%s') n2:%p('%s') n3:%p\n", new_1, new_1, new_2, new_2, new_3);

    free(new_1);
    free(new_2);

    return 0;
}