#ifndef __A2E_SERVER_H__
#define __A2E_SERVER_H__

#include "a2e_def.h"
#include "a2e_iface.h"

typedef struct
{
    a2e_t base;
    a2e_strategy_i iface;
    void *req;
} a2e_server_t;

a2e_strategy_i a2e_server_iface_get(void);

#endif /* __A2E_SERVER_H__ */
