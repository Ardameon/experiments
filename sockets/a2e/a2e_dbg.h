#ifndef __A2E_DBG_H__
#define __A2E_DBG_H__

#include <stdint.h>

#include "a2e_def.h"

#ifndef NDEBUG
/** @brief Debug log */
#define A2E_DBG(format, ...) a2e_log(format, ##__VA_ARGS__)
#else
/** @brief Debug log */
#define A2E_DBG(format, ...)
#endif

void a2e_log(const char *format, ...);

void a2e_set_dbg(uint8_t on);
void a2e_set_dbg_log_func(a2e_log_func log_func);

#endif /* __A2E_DBG_H__ */
