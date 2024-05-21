#ifndef _FLY_BEHAVIOR_
#define _FLY_BEHAVIOR_

typedef void (*fly_func)(void);

typedef struct
{
    fly_func fly;
} fly_behavior_i;

void fly_with_wings(void);
void fly_no_way(void);
void fly_with_rocket(void);

static const fly_behavior_i fly_with_wings_imp  = { &fly_with_wings  };
static const fly_behavior_i fly_no_way_imp      = { &fly_no_way      };
static const fly_behavior_i fly_with_rocket_imp = { &fly_with_rocket };

#endif /*_FLY_BEHAVIOR_*/
