#include "duck.h"

void duck_handle(duck_t *duck)
{
    duck_show(duck);
    duck_fly(duck);
    duck_quak(duck);
    duck_destroy(duck);
}

int main()
{
    duck_t *duck;

    mellor_duck_create(&duck);
    duck_handle(duck);
    rocket_duck_create(&duck);
    duck_handle(duck);
    bath_duck_create(&duck);
    duck_handle(duck);

    return 0;
}
