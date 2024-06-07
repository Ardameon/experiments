#include "array.h"

int array_size(const array_t *array)
{
    if (array && array->size_strategy.size)
        return array->size_strategy.size(array);

    return -1;
}

void array_fill(array_t *array)
{
    if (array && array->fill_strategy.fill)
        array->fill_strategy.fill(array);
}

void array_sort(array_t *array)
{
    if (array && array->sort_strategy.sort)
        array->sort_strategy.sort(array);
}

int array_find(const array_t *array, int elem)
{
    if (array && array->find_strategy.find)
        return array->find_strategy.find(array, elem);

    return -1;
}

void array_show(const array_t *array)
{
    if (array && array->show_strategy.show)
        array->show_strategy.show(array);
}
