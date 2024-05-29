#include "array.h"

int array_size(const array_t *array)
{
    if (array && array->size_strategy.size_func)
        return array->size_strategy.size_func(array);

    return -1;
}

void array_fill(array_t *array)
{
}

void array_sort(array_t *array)
{
}

int array_find(const array_t *array, int elem)
{
    return -1;
}

void array_show(const array_t *array)
{
}
