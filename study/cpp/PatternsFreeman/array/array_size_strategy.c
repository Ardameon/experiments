#include "array_size_strategy.h"
#include "array.h"

int array_size_default(const array_t *arr)
{
    return arr->size;
}
