#ifndef __ARRAY_FILL_STRATEGY__
#define __ARRAY_FILL_STRATEGY__

typedef struct array_t array_t;

typedef void (*array_fill_func)(array_t *arr);

typedef struct array_fill_strategy_i
{
    array_fill_func fill;
} array_fill_strategy_i;

void array_fill_random(array_t *arr);
void array_fill_inc_pos(array_t *arr);
void array_fill_inc_neg(array_t *arr);
void array_fill_inc_mix(array_t *arr);
void array_fill_dec_pos(array_t *arr);
void array_fill_dec_neg(array_t *arr);
void array_fill_dec_mix(array_t *arr);

static array_fill_strategy_i fill_array_random  = { &array_fill_random };
static array_fill_strategy_i fill_array_inc_pos = { &array_fill_inc_pos };
static array_fill_strategy_i fill_array_inc_neg = { &array_fill_inc_neg };
static array_fill_strategy_i fill_array_inc_mix = { &array_fill_inc_mix };
static array_fill_strategy_i fill_array_dec_pos = { &array_fill_dec_pos };
static array_fill_strategy_i fill_array_dec_neg = { &array_fill_dec_neg };
static array_fill_strategy_i fill_array_dec_mix = { &array_fill_dec_mix };


#endif /*__ARRAY_FILL_STRATEGY__*/
