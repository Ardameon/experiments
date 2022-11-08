#include <stdio.h>
#include <stdlib.h>
#include "cadapter.h"

void quack(IDuck *iduck)
{
    if (iduck && iduck->quack_func)
        iduck->quack_func(iduck);
}

void fly(IDuck *iduck)
{
    if (iduck && iduck->fly_func)
        iduck->fly_func(iduck);
}

Duck *duck_create(void)
{
    Duck *duck = malloc(sizeof(*duck));

    duck->base.fly_func   = &duck_fly;
    duck->base.quack_func = &duck_quack;

    return duck;
}

void duck_destroy(Duck *duck)
{
    if (duck)
        free(duck);
}

void duck_quack(struct IDuck *iduck)
{
    printf("Quack as duck!\n");
}

void duck_fly(struct IDuck *iduck)
{
    printf("Fly as duck\n");
}

Plane *plane_create(void)
{
    Plane *plane = malloc(sizeof(*plane));

    plane->fly_func    = &plane_fly;
    plane->rumble_func = &plane_rumble;

    return plane;
}

void plane_destroy(Plane *plane)
{
    if (plane)
        free(plane);
}

void plane_rumble(void)
{
    printf("Rumble as plane!\n");
}

void plane_fly(void)
{
    printf("Fly as plane\n");
}

PlaneAdapter *plane_adapter_create(Plane *plane)
{
    PlaneAdapter *pa = malloc(sizeof(*pa));

    pa->base.fly_func   = &plane_adapter_fly;
    pa->base.quack_func = &plane_adapter_quack;

    pa->plane = plane;

    return pa;
}

void plane_adapter_destroy(PlaneAdapter *adapter)
{
    if (adapter)
        free(adapter);
}

void plane_adapter_fly(struct IDuck *iduck)
{
    PlaneAdapter *pa = iduck;

    if (pa->plane && pa->plane->fly_func)
        pa->plane->fly_func();
}

void plane_adapter_quack(struct IDuck *iduck)
{
    PlaneAdapter *pa = iduck;

    if (pa->plane && pa->plane->rumble_func)
        pa->plane->rumble_func();
}











