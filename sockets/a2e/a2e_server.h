#ifndef __A2E_SERVER_H__
#define __A2E_SERVER_H__

#include "a2e_def.h"
#include "a2e_iface.h"
#include "a2e_common.h"

typedef struct
{
    a2e_t base;
    a2e_strategy_i iface;

    char sock_path[A2E_SOCK_PATH_LEN_MAX + 1];

    int fd_listen;
    int fd_rw;

    uint8_t *req;
    uint32_t req_size_recv;
    uint32_t req_size_exp;
    uint8_t  req_cmplt_wait_tries;

    uint32_t rsp_size_sent;
} a2e_server_t;

a2e_strategy_i a2e_server_iface_get(void);

#endif /* __A2E_SERVER_H__ */

