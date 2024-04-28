#include <stdio.h>
#include <stdlib.h>

#include "rocket_duck.h"
#include "fly_behavior.h"
#include "quak_behavior.h"

void rocket_duck_destroy(rocket_duck_t *duck);
void rocket_duck_show   (rocket_duck_t *duck);

void rocket_duck_create (duck_t **duck)
{
    rocket_duck_t *new_duck = (rocket_duck_t *)(*duck);

    new_duck = (rocket_duck_t *)malloc(sizeof(*new_duck));

    duck_create(&new_duck->base);

    new_duck->base.fly_behaviour  = fly_with_rocket_imp;
    new_duck->base.quak_behaviour = silent_quak_imp;

    new_duck->base.duck_show    = (duck_show_func)   &rocket_duck_show;
    new_duck->base.duck_destroy = (duck_destroy_func)&rocket_duck_destroy;

    (*duck) = (duck_t *)new_duck;

    printf("Rocket duck created (%p size: %zd)\n", new_duck, sizeof(*new_duck));
}

void rocket_duck_destroy(rocket_duck_t *duck)
{
    printf("Rocket duck destroyed (%p)\n", duck);
    free(duck);
}

void rocket_duck_show(rocket_duck_t *duck)
{
    printf("I'm Rocket Duck! (%p)\n", duck);
}

