#ifndef __ARRAY_SHOW_STRATEGY__
#define __ARRAY_SHOW_STRATEGY__

typedef struct array_t array_t;

typedef void (*array_show_func)(const array_t *arr);

typedef struct array_show_strategy_i
{
    array_show_func show;
} array_show_strategy_i;

void array_show_default(const array_t *arr);

static array_show_strategy_i show_array = { &array_show_default };

#endif /*__ARRAY_SHOW_STRATEGY__*/
