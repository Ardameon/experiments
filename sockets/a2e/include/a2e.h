#ifndef __A2E_H__
#define __A2E_H__

#include <stdint.h>

#include "a2e_def.h"

a2e_status_e a2e_init_client(a2e_t **a2e, const a2e_cfg_t *cfg);
a2e_status_e a2e_init_server(a2e_t **a2e, const a2e_cfg_t *cfg);

a2e_status_e a2e_close(a2e_t *a2e);

a2e_status_e a2e_request_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
a2e_status_e a2e_request_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);
a2e_status_e a2e_request_complete(a2e_t *a2e);
a2e_status_e a2e_request_complete_wait(a2e_t *a2e, uint16_t to_ms);

a2e_status_e a2e_response_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
a2e_status_e a2e_response_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);

a2e_status_e a2e_progress_tx(a2e_t *a2e, uint16_t to_ms);

void a2e_cfg_set_default(a2e_cfg_t *cfg);

void a2e_dbg_on(void);
void a2e_dbg_off(void);
void a2e_dbg_set_func(a2e_log_func log_func);

const char *a2e_perror(a2e_status_e status);


#endif /* __A2E_H__ */
