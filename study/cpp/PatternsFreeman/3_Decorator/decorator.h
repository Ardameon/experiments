/*=========================================================*/
/* Abstract object */
typedef struct string_getter_abs_t
{
    const char *(*string_get)(struct string_getter_abs_t *str_getter);
} string_getter_abs_t;

/*=========================================================*/
/* Original object to decorate */
typedef struct string_getter_t
{
    string_getter_abs_t str_getter;
} string_getter_t;

/*=========================================================*/
/* Abstract decorator */
typedef struct string_getter_dec_abs_t
{
    string_getter_abs_t str_getter;
    string_getter_abs_t *decorated;
} string_getter_dec_abs_t;

typedef struct string_getter_flower_t
{
    string_getter_dec_abs_t decorator;
} string_getter_flower_t;

typedef struct string_getter_wind_t
{
    string_getter_dec_abs_t decorator;
} string_getter_wind_t;

typedef struct string_getter_sand_t
{
    string_getter_dec_abs_t decorator;
} string_getter_sand_t;
