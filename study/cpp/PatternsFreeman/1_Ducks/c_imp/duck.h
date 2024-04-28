#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*fly_func)(void);
typedef void (*quak_func)(void);

typedef struct
{
    fly_func fly;
} fly_behavior_i;

typedef struct
{
    quak_func quak;
} quak_behavior_i;

void fly_with_wings(void)
{
    printf("Fly with wings\n");
}

void fly_no_way(void)
{
    printf("Can't fly\n");
}

void fly_with_rocket(void)
{
    printf("Fly with rocket\n");
}

const fly_behavior_i fly_with_wings_imp  = { &fly_with_wings };
const fly_behavior_i fly_no_way_imp      = { &fly_no_way };
const fly_behavior_i fly_with_rocket_imp = { &fly_with_rocket };

void quak(void)
{
    printf("Quack\n");
}

void squeek(void)
{
    printf("Squeek\n");
}

void silent_quak(void)
{
    printf("Silent_quak\n");
}

const quak_behavior_i quak_imp        = { &quak };
const quak_behavior_i squeek_imp      = { &squeek };
const quak_behavior_i silent_quak_imp = { &silent_quak };

typedef struct duck_t duck_t;

typedef void (*duck_show_func)(void);
typedef void (*duck_destroy_func)(duck_t *duck);

typedef struct duck_t
{
    fly_behavior_i fly_behaviour;
    quak_behavior_i quak_behaviour;

    duck_show_func    duck_show;
    duck_destroy_func duck_destroy;
} duck_t;

void duck_create(duck_t *duck);
void duck_destroy(duck_t *duck);
void duck_show(duck_t *duck);
void duck_fly(duck_t *duck);
void duck_quak(duck_t *duck);

typedef struct
{
    duck_t base;
} mellor_duck_t;

void mellor_duck_create (duck_t **duck);
void mellor_duck_destroy(mellor_duck_t *duck);
void mellor_duck_show   (mellor_duck_t *duck);

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

typedef struct
{
    duck_t base;
} rocket_duck_t;

void rocket_duck_create (duck_t **duck);
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

typedef struct
{
    duck_t base;
} bath_duck_t;

void bath_duck_create (duck_t **duck);
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


