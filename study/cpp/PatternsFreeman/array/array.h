#ifndef __ARRAY__
#define __ARRAY__

#include "array_size_strategy.h"

typedef struct array_t array_t;

typedef struct array_t
{
    array_size_strategy_i size_strategy;
} array_t;

int  array_size(const array_t *array);
void array_fill(array_t *array);
void array_sort(array_t *array);
int  array_find(const array_t *array, int elem);
void array_show(const array_t *array);

#endif /* __ARRAY__ */
