#ifndef __A2E_H__
#define __A2E_H__

#include <stdint.h>

#include "a2e_def.h"

a2e_status_e a2e_init_client(a2e_t **a2e);
a2e_status_e a2e_init_server(a2e_t **a2e);

a2e_status_e a2e_close(a2e_t *a2e);

a2e_status_e a2e_request_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size);
a2e_status_e a2e_request_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size);
a2e_status_e a2e_request_complete(a2e_t *a2e);

a2e_status_e a2e_response_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size);
a2e_status_e a2e_response_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size);

a2e_status_e a2e_progress_tx(a2e_t *a2e);

void a2e_set_dbg_on(void);
void a2e_set_dbg_off(void);
void a2e_set_dbg_func(a2e_log_func log_func);

const char *a2e_perror(a2e_status_e status);

#endif /* __A2E_H__ */
