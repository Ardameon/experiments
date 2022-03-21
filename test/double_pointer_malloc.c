#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    void **parr;

    parr = malloc(10 * sizeof(void *));

    // parr = NULL;

    *parr = 13123;

    // free(parr);

    // free(parr);
    free(parr);

    printf("UPADI\n!");


    return 0;
}