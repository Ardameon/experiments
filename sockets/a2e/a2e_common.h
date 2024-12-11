#ifndef __A2E_COMMON_H__
#define __A2E_COMMON_H__

#include <stdint.h>

#include "a2e_def.h"

#define DEF_A2E_SOCK_DIR  "/tmp"
#define DEF_A2E_SOCK_NAME "a2e.sock"
#define DEF_A2E_NAME      "A2E"

#define DEF_A2E_RW_CHUNK_SIZE 16
#define DEF_A2E_RW_POLL_TIMEOUT_MS 50
#define DEF_A2E_REQ_CMPLT_WAIT_TRIES 3

#define A2E_SOCK_PATH_LEN_MAX 96

#define A2E_MIN(a, b) ((a) < (b) ? (a) : (b))
#define A2E_BASE(x) ((a2e_t *)x)

unsigned long a2e_get_timestamp(void);
uint8_t a2e_timeout_reached(unsigned long start_timestamp, unsigned timeout_ms);

void a2e_set_state(a2e_t *a2e, a2e_state_e state);

const char *a2e_state_str(a2e_state_e state);

const char *a2e_name(const a2e_t *a2e);

void a2e_cfg_apply(a2e_t *a2e, const a2e_cfg_t *cfg);

#endif /* __A2E_COMMON_H__ */
