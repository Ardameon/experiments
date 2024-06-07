#ifndef __ARRAY_FILL_STRATEGY__
#define __ARRAY_FILL_STRATEGY__

typedef struct array_t array_t;

typedef void (*array_fill_func)(array_t *arr);

typedef struct array_fill_strategy_i
{
    array_fill_func fill;
} array_fill_strategy_i;

void array_fill_default(array_t *arr);

static array_fill_strategy_i fill_array = { &array_fill_default };

#endif /*__ARRAY_FILL_STRATEGY__*/
