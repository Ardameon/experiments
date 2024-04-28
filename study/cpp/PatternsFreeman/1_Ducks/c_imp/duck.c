#include <string.h>

#include "duck.h"

void duck_create(duck_t *duck)
{
    memset(duck, 0, sizeof(*duck));
}

void duck_destroy(duck_t *duck)
{
    duck->duck_destroy(duck);
}

void duck_show(duck_t *duck)
{
    duck->duck_show();
}

void duck_fly(duck_t *duck)
{
    duck->fly_behaviour.fly();
}

void duck_quak(duck_t *duck)
{
    duck->quak_behaviour.quak();
}
