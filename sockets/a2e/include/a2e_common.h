/**
 * @file
 *
 * @defgroup a2e_internal A2E library internals
 *
 * @brief Library private data structures, types and functions
 *
 * @ingroup A2E
 *
 * @defgroup a2e_common Common
 *
 * @brief Library common functions and types used all over the place
 *
 * @ingroup a2e_internal
 * @{
 */

#ifndef __A2E_COMMON_H__
#define __A2E_COMMON_H__

#include <stdint.h>

#include "a2e_def.h"

/** @brief Default UNIX socket path */
#define DEF_A2E_SOCK_DIR  "/tmp"
/** @brief Default UNIX socket name */
#define DEF_A2E_SOCK_NAME "a2e.sock"
/** @brief Default A2E-instance name */
#define DEF_A2E_NAME      "A2E"

/** @brief Default RX/TX data chunk size */
#define DEF_A2E_RW_CHUNK_SIZE (64 * 1024)
/** @brief Default socket poll timeout */
#define DEF_A2E_RW_POLL_TIMEOUT_MS 50
/** @brief Default request complete wait tries count for server */
#define DEF_A2E_REQ_CMPLT_WAIT_TRIES 3

/** @brief System maximum socket path length */
#define A2E_SOCK_PATH_LEN_MAX 96

/** @brief Macro to get min value of two comparables */
#define A2E_MIN(a, b) ((a) < (b) ? (a) : (b))
/** @brief Macro to get base @ref a2e_t "A2E-instance" */
#define A2E_BASE(x) ((a2e_t *)x)

/** @brief Get current time from system start in milliseconds */
unsigned long a2e_get_timestamp(void);
/** @brief Check if timeout since start timestamp is reached */
uint8_t a2e_timeout_reached(unsigned long start_timestamp, unsigned timeout_ms);

/** @brief Set A2E-instanse state */
void a2e_set_state(a2e_t *a2e, a2e_state_e state);

/** @brief Get string implementation of A2E-instance state */
const char *a2e_state_str(a2e_state_e state);

/** @brief Get A2E-instance name */
const char *a2e_name(const a2e_t *a2e);

/** @brief Apply config for A2E-instance */
void a2e_cfg_apply(a2e_t *a2e, const a2e_cfg_t *cfg);

#endif /* __A2E_COMMON_H__ */

/** @} */
