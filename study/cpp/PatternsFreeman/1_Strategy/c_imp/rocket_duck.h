#ifndef _ROCKET_DUCK_
#define _ROCKET_DUCK_

#include "duck.h"

typedef struct
{
    duck_t base;
} rocket_duck_t;

void rocket_duck_create(duck_t **duck);

#endif /*_ROCKET_DUCK_*/
