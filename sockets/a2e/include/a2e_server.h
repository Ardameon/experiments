/**
 * @file
 *
 * @defgroup a2e_server Server
 *
 * @brief UNIX stream socket server implementation
 *
 * @ingroup a2e_internal
 * @{
 */

#ifndef __A2E_SERVER_H__
#define __A2E_SERVER_H__

#include "a2e_def.h"
#include "a2e_iface.h"
#include "a2e_common.h"

/** @brief A2E-server-instance */
typedef struct
{
    /** Base A2E-instance object */
    a2e_t base;
    /** Inteface object */
    a2e_strategy_i iface;

    /** Instance socket path */
    char sock_path[A2E_SOCK_PATH_LEN_MAX + 1];

    /** Connection socket file descriptor */
    int fd_listen;
    /** RX/TX socket file descriptor */
    int fd_rw;

    /** Pointer to received reqeust */
    uint8_t *req;
    /** Size of receive reqeust */
    uint32_t req_size_recv;
    /** Expected size of request */
    uint32_t req_size_exp;
    /** Request complete wait tries counter */
    uint8_t  req_cmplt_wait_tries;

    /** Size of sent response */
    uint32_t rsp_size_sent;
} a2e_server_t;

/** @brief Get A2E-server-instance interface */
a2e_strategy_i a2e_server_iface_get(void);

#endif /* __A2E_SERVER_H__ */

/** @} */

