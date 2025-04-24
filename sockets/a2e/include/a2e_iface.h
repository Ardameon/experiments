/**
 * @file
 *
 * @defgroup a2e_iface Instance interface
 *
 * @brief Virtual functions table interface for all A2E-instances (strategy pattern)
 *
 * @ingroup a2e_internal
 * @{
 */

#ifndef __A2E_IFACE_H__
#define __A2E_IFACE_H__

#include <stdint.h>

/** @brief Forward declaration */
typedef struct a2e_t a2e_t;
/** @brief Forward declaration */
typedef struct a2e_cfg_t a2e_cfg_t;

/**
 * @name Function pointers for A2E-instance interface functions
 * @{
 */
/** @brief Request RX function pointer */
typedef int (*a2e_request_rx_func)           (a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
/** @brief Request TX function pointer */
typedef int (*a2e_request_tx_func)           (a2e_t *a2e, uint8_t  *tx_buffer, uint32_t size, uint16_t to_ms);
/** @brief Request complete wait function pointer */
typedef int (*a2e_request_complete_wait_func)(a2e_t *a2e, uint16_t to_ms);
/** @brief Request complete function pointer */
typedef int (*a2e_request_complete_func)     (a2e_t *a2e);
/** @brief Response RX function pointer */
typedef int (*a2e_response_rx_func)          (a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
/** @brief Response TX function pointer */
typedef int (*a2e_response_tx_func)          (a2e_t *a2e, uint8_t  *tx_buffer, uint32_t size, uint16_t to_ms);
/** @brief Progress TX function pointer */
typedef int (*a2e_progress_tx_func)          (a2e_t *a2e, uint16_t to_ms);
/** @brief Instance init function pointer */
typedef int (*a2e_init_func)                 (a2e_t **a2e, const a2e_cfg_t *cfg);
/** @brief Instance close function pointer */
typedef int (*a2e_close_func)                (a2e_t *a2e);
/** @} */

/** @brief Strategy interface structure */
typedef struct a2e_strategy_i
{
    /** init instance */
    a2e_init_func                  init;
    /** close instance */
    a2e_close_func                 close;
    /** RX request */
    a2e_request_rx_func            req_rx;
    /** TX request */
    a2e_request_tx_func            req_tx;
    /** Complete request */
    a2e_request_complete_func      req_cmplt;
    /** Wait request complete */
    a2e_request_complete_wait_func req_cmplt_wait;
    /** RX response */
    a2e_response_rx_func           resp_rx;
    /** TX response */
    a2e_response_tx_func           resp_tx;
    /** TX progress */
    a2e_progress_tx_func           prog_tx;
} a2e_strategy_i;

#endif /* __A2E_IFACE_H__ */

/** @} */

