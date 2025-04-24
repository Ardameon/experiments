/**
 * @file
 *
 * @defgroup a2e_client Client
 *
 * @brief UNIX stream socket client implementation
 *
 * @ingroup a2e_internal
 * @{
 */
#ifndef __A2E_CLIENT_H__
#define __A2E_CLIENT_H__

#include "a2e_def.h"
#include "a2e_iface.h"
#include "a2e_common.h"

/** @brief A2E-client-instance */
typedef struct
{
    /** Base A2E-instance object */
    a2e_t base;
    /** Inteface object */
    a2e_strategy_i iface;

    /** Instance socket path */
    char sock_path[A2E_SOCK_PATH_LEN_MAX + 1];

    /** Socket file descriptor */
    int fd;

    /** Pointer to received response */
    uint8_t *rsp;
    /** Size of receive response */
    uint32_t rsp_size_recv;
    /** Expected size of response */
    uint32_t rsp_size_exp;

    /** Size of sent request */
    uint32_t req_size_sent;
} a2e_client_t;

/** @brief Get A2E-client-instance interface */
a2e_strategy_i a2e_client_iface_get(void);

#endif /* __A2E_CLIENT_H__ */

/** @} */
