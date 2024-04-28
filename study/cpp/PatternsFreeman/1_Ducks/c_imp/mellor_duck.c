#include <stdio.h>
#include <stdlib.h>

#include "mellor_duck.h"
#include "fly_behavior.h"
#include "quak_behavior.h"

static void mellor_duck_destroy(mellor_duck_t *duck);
static void mellor_duck_show   (mellor_duck_t *duck);

void mellor_duck_create(duck_t **duck)
{
    mellor_duck_t *new_duck = (mellor_duck_t *)(*duck);

    new_duck = (mellor_duck_t *)malloc(sizeof(*new_duck));

    duck_create(&new_duck->base);

    new_duck->base.fly_behaviour  = fly_with_wings_imp;
    new_duck->base.quak_behaviour = quak_imp;

    new_duck->base.duck_show    = (duck_show_func)   &mellor_duck_show;
    new_duck->base.duck_destroy = (duck_destroy_func)&mellor_duck_destroy;

    (*duck) = (duck_t *)new_duck;

    printf("Mellor duck created (%p size: %zd)\n", new_duck, sizeof(*new_duck));
}

void mellor_duck_destroy(mellor_duck_t *duck)
{
    printf("Mellor duck destroyed (%p)\n", duck);
    free(duck);
}

void mellor_duck_show(mellor_duck_t *duck)
{
    printf("I'm Mellor Duck! (%p)\n", duck);
}
