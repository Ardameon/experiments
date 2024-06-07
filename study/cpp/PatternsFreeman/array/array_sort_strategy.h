#ifndef __ARRAY_SORT_STRATEGY__
#define __ARRAY_SORT_STRATEGY__

typedef struct array_t array_t;

typedef void (*array_sort_func)(array_t *array);

typedef struct array_sort_strategy_i
{
    array_sort_func sort;
} array_sort_strategy_i;

void array_sort_default(array_t *array);

static array_sort_strategy_i sort_array = { &array_sort_default };

#endif /*__ARRAY_SORT_STRATEGY__*/
