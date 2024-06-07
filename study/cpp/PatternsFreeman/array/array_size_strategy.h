#ifndef __ARRAY_SIZE_STRATEGY__
#define __ARRAY_SIZE_STRATEGY__

typedef struct array_t array_t;

typedef int (*array_size_func)(const array_t *arr);

typedef struct array_size_strategy_i
{
    array_size_func size;
} array_size_strategy_i;

int array_size_default(const array_t *arr);

static array_size_strategy_i size_array = { &array_size_default };

#endif /*__ARRAY_SIZE_STRATEGY__*/
