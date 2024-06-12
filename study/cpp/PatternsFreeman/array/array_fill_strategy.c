#include "array_fill_strategy.h"
#include "array.h"

#include <stdlib.h>
#include <time.h>

#define RANDOM_RANGE_SIZE 200

void array_fill_random(array_t *arr)
{
    int size, i;

    srand(time(0));
    size = array_size(arr);

    for (i = 0; i < size; i++)
    {
        arr->item[i] = (rand() % RANDOM_RANGE_SIZE) - (RANDOM_RANGE_SIZE / 2);
    }
}

void array_fill_inc_pos(array_t *arr) {}
void array_fill_inc_neg(array_t *arr) {}
void array_fill_inc_mix(array_t *arr) {}
void array_fill_dec_pos(array_t *arr) {}
void array_fill_dec_neg(array_t *arr) {}
void array_fill_dec_mix(array_t *arr) {}
