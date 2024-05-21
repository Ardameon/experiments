#ifndef _MELLOR_DUCK_
#define _MELLOR_DUCK_

#include "duck.h"

typedef struct
{
    duck_t base;
} mellor_duck_t;

void mellor_duck_create(duck_t **duck);

#endif /*_MELLOR_DUCK_*/
