#ifndef __ARRAY_FIND_STRATEGY__
#define __ARRAY_FIND_STRATEGY__

typedef struct array_t array_t;

typedef int (*array_find_func)(const array_t *array, int elem);

typedef struct array_find_strategy_i
{
    array_find_func find;
} array_find_strategy_i;

int array_find_default(const array_t *array, int elem);

static array_find_strategy_i find_array = { &array_find_default };

#endif /*__ARRAY_FIND_STRATEGY__*/
