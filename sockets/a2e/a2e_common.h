#ifndef __A2E_COMMON_H__
#define __A2E_COMMON_H__

#include "a2e_def.h"

#define DEF_A2E_SOCK_DIR  "/tmp"
#define DEF_A2E_SOCK_NAME "a2e.sock"
#define DEF_A2E_NAME      "A2E"

unsigned long a2e_get_timestamp(void);

void a2e_set_state(a2e_t *a2e, a2e_state_e state);

const char *a2e_state_str(a2e_state_e state);

#endif /* __A2E_COMMON_H__ */
