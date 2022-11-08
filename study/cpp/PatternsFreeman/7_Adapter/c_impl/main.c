#include <stdio.h>
#include "cadapter.h"

int main()
{
    Duck *duck = duck_create();
    Plane *plane = plane_create();
    PlaneAdapter *planoduck = plane_adapter_create(plane);

    quack((IDuck *)duck);
    fly((IDuck *)duck);

    quack((IDuck *)planoduck);
    fly((IDuck *)planoduck);

    duck_destroy(duck);
    plane_destroy(plane);
    plane_adapter_destroy(planoduck);

    return 0;
}
