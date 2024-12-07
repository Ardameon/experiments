#ifndef __A2E_CLIENT_H__
#define __A2E_CLIENT_H__

#include "a2e_def.h"
#include "a2e_iface.h"
#include "a2e_common.h"

typedef struct
{
    a2e_t base;
    a2e_strategy_i iface;

    char sock_path[A2E_SOCK_PATH_LEN_MAX + 1];

    int fd;

    uint8_t *resp;
    uint32_t resp_size_recv;
    uint32_t resp_size_exp;

} a2e_client_t;

a2e_strategy_i a2e_client_iface_get(void);

#endif /* __A2E_CLIENT_H__ */
