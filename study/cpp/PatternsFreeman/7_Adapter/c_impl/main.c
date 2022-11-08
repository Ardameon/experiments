#include <stdio.h>
#include "cadapter.h"

int main()
{
    Duck *duck = duck_create();
    Plane *plane = plane_create();
    PlaneAdapter *planoduck = plane_adapter_create(plane);

    quack(duck);
    fly(duck);

    quack(planoduck);
    fly(planoduck);

    duck_destroy(duck);
    plane_destroy(plane);
    plane_adapter_destroy(planoduck);

    return 0;
}
