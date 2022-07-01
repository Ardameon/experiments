#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "decorator.h"

/*=========================================================*/

void        str_getter_init(string_getter_t *str_getter);
const char *str_getter_string_get(string_getter_abs_t *str_getter);
void        str_getter_flower_init(string_getter_flower_t *str_getter, string_getter_abs_t *decorated);
const char *str_getter_flower_string_get(string_getter_abs_t *str_getter);
void        str_getter_wind_init(string_getter_wind_t *str_getter, string_getter_abs_t *decorated);
const char *str_getter_wind_string_get(string_getter_abs_t *str_getter);
void        str_getter_sand_init(string_getter_sand_t *str_getter, string_getter_abs_t *decorated);
const char *str_getter_sand_string_get(string_getter_abs_t *str_getter);

/*=========================================================*/

int main(int argc, char const *argv[])
{
    string_getter_t        default_getter;
    string_getter_flower_t flower_getter;
    string_getter_wind_t   wind_getter;
    string_getter_sand_t   sand_getter;

    str_getter_init(&default_getter);
    str_getter_flower_init(&flower_getter, (string_getter_abs_t*)&default_getter);
    str_getter_wind_init(&wind_getter, (string_getter_abs_t*)&flower_getter);
    str_getter_sand_init(&sand_getter, (string_getter_abs_t*)&wind_getter);

    printf("str: '%s'\n", ((string_getter_abs_t*)&sand_getter)->string_get((string_getter_abs_t*)&sand_getter));
    fflush(stdout);

    (void)argv;
    (void)argc;
    return 0;

}

/*=========================================================*/

void str_getter_init(string_getter_t *str_getter)
{
    ((string_getter_abs_t *)str_getter)->string_get = &str_getter_string_get;
}

const char *str_getter_string_get(string_getter_abs_t *str_getter)
{
    (void)str_getter;

    return "base-string";
}

/*=========================================================*/

void str_getter_flower_init(string_getter_flower_t *str_getter, string_getter_abs_t *decorated)
{
    ((string_getter_dec_abs_t *)str_getter)->decorated = decorated;
    ((string_getter_abs_t *)str_getter)->string_get    = &str_getter_flower_string_get;
}

const char *str_getter_flower_string_get(string_getter_abs_t *str_getter)
{
    string_getter_abs_t *decorated = ((string_getter_dec_abs_t *)str_getter)->decorated;
    
    char *string = malloc(256);
    sprintf(string, "%s %s", "flower", decorated->string_get(decorated));

    return string;
}

/*=========================================================*/

void str_getter_wind_init(string_getter_wind_t *str_getter, string_getter_abs_t *decorated)
{
    ((string_getter_dec_abs_t *)str_getter)->decorated = decorated;
    ((string_getter_abs_t *)str_getter)->string_get    = &str_getter_wind_string_get;
}

const char *str_getter_wind_string_get(string_getter_abs_t *str_getter)
{
    string_getter_abs_t *decorated = ((string_getter_dec_abs_t *)str_getter)->decorated;
    
    char *string = malloc(256);
    sprintf(string, "%s %s", "wind", decorated->string_get(decorated));

    return string;
}

/*=========================================================*/

void str_getter_sand_init(string_getter_sand_t *str_getter, string_getter_abs_t *decorated)
{
    ((string_getter_dec_abs_t *)str_getter)->decorated = decorated;
    ((string_getter_abs_t *)str_getter)->string_get    = &str_getter_sand_string_get;
}

const char *str_getter_sand_string_get(string_getter_abs_t *str_getter)
{
    string_getter_abs_t *decorated = ((string_getter_dec_abs_t *)str_getter)->decorated;
    
    char *string = malloc(256);
    sprintf(string, "%s %s", "sand", decorated->string_get(decorated));

    return string;
}

/*=========================================================*/


