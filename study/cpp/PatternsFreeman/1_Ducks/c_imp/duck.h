#ifndef _DUCK_
#define _DUCK_

#include "fly_behavior.h"
#include "quak_behavior.h"

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

#endif /*_DUCK_*/

