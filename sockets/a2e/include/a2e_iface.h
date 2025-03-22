#ifndef __A2E_IFACE_H__
#define __A2E_IFACE_H__

#include <stdint.h>

typedef struct a2e_t a2e_t;
typedef struct a2e_cfg_t a2e_cfg_t;

typedef int (*a2e_request_rx_func)           (a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
typedef int (*a2e_request_tx_func)           (a2e_t *a2e, uint8_t  *tx_buffer, uint32_t size, uint16_t to_ms);
typedef int (*a2e_request_complete_wait_func)(a2e_t *a2e, uint16_t to_ms);
typedef int (*a2e_request_complete_func)     (a2e_t *a2e);

typedef int (*a2e_response_rx_func)          (a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
typedef int (*a2e_response_tx_func)          (a2e_t *a2e, uint8_t  *tx_buffer, uint32_t size, uint16_t to_ms);

typedef int (*a2e_progress_tx_func)          (a2e_t *a2e, uint16_t to_ms);

typedef int (*a2e_init_func)                 (a2e_t **a2e, const a2e_cfg_t *cfg);
typedef int (*a2e_close_func)                (a2e_t *a2e);

typedef struct a2e_strategy_i
{
    a2e_init_func                  init;
    a2e_close_func                 close;
    a2e_request_rx_func            req_rx;
    a2e_request_tx_func            req_tx;
    a2e_request_complete_wait_func req_cmplt_wait;
    a2e_request_complete_func      req_cmplt;
    a2e_response_rx_func           resp_rx;
    a2e_response_tx_func           resp_tx;
    a2e_progress_tx_func           prog_tx;
} a2e_strategy_i;

#endif /* __A2E_IFACE_H__ */
