#include <stdio.h>

#include "duck.h"
#include "mellor_duck.h"
#include "rocket_duck.h"
#include "bath_duck.h"

void duck_handle(duck_t *duck)
{
    duck_show(duck);
    duck_fly(duck);
    duck_quak(duck);
    duck_destroy(duck);
}

int main()
{
    const int cnt = 3;
    duck_t *duck[cnt];
    int i;

    mellor_duck_create(&duck[0]);
    rocket_duck_create(&duck[1]);
    bath_duck_create(&duck[2]);

    printf("==============================\n");

    for (i = 0; i < cnt; i++)
    {
        duck_handle(duck[i]);
    }


    return 0;
}
