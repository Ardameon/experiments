#ifndef _QUAK_BEHAVIOR_
#define _QUAK_BEHAVIOR_

typedef void (*quak_func)(void);

typedef struct
{
    quak_func quak;
} quak_behavior_i;

void quak(void);
void squeek(void);
void silent_quak(void);

static const quak_behavior_i quak_imp        = { &quak        };
static const quak_behavior_i squeek_imp      = { &squeek      };
static const quak_behavior_i silent_quak_imp = { &silent_quak };

#endif /*_QUAK_BEHAVIOR_*/
