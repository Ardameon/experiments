#ifndef __ARRAY__
#define __ARRAY__

#include <stdlib.h>

#include "array_size_strategy.h"
#include "array_fill_strategy.h"
#include "array_sort_strategy.h"
#include "array_find_strategy.h"
#include "array_show_strategy.h"


typedef struct array_t
{
    array_size_strategy_i size_strategy;
    array_fill_strategy_i fill_strategy;
    array_sort_strategy_i sort_strategy;
    array_find_strategy_i find_strategy;
    array_show_strategy_i show_strategy;

    int *item;
    size_t size;
} array_t;

int  array_size(const array_t *array);
void array_fill(array_t *array);
void array_sort(array_t *array);
int  array_find(const array_t *array, int elem);
void array_show(const array_t *array);

#endif /* __ARRAY__ */
