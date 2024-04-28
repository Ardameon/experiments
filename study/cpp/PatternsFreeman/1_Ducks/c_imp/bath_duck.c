#include <stdio.h>
#include <stdlib.h>

#include "bath_duck.h"
#include "fly_behavior.h"
#include "quak_behavior.h"

void bath_duck_destroy(bath_duck_t *duck);
void bath_duck_show   (bath_duck_t *duck);

void bath_duck_create (duck_t **duck)
{
    bath_duck_t *new_duck = (bath_duck_t *)(*duck);
    new_duck = (bath_duck_t *)malloc(sizeof(*new_duck));

    duck_create(&new_duck->base);

    new_duck->base.fly_behaviour  = fly_no_way_imp;
    new_duck->base.quak_behaviour = squeek_imp;

    new_duck->base.duck_show    = (duck_show_func)   &bath_duck_show;
    new_duck->base.duck_destroy = (duck_destroy_func)&bath_duck_destroy;

    (*duck) = (duck_t *)new_duck;

    printf("Bath duck created (%p size: %zd)\n", new_duck, sizeof(*new_duck));
}

void bath_duck_destroy(bath_duck_t *duck)
{
    printf("Bath duck destroyed (%p)\n", duck);
    free(duck);
}

void bath_duck_show(bath_duck_t *duck)
{
    printf("I'm Bath Duck! (%p)\n", duck);
}

