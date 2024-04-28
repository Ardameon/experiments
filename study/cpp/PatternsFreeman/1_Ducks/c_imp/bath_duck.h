#ifndef _BATH_DUCK_
#define _BATH_DUCK_

#include "duck.h"

typedef struct
{
    duck_t base;
} bath_duck_t;

void bath_duck_create(duck_t **duck);

#endif /*_BATH_DUCK_*/

