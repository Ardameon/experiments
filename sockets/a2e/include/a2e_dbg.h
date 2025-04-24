/**
 * @file
 *
 * @defgroup a2e_dbg Debug
 *
 * @brief Library debug logic
 *
 * @ingroup a2e_internal
 * @{
 */

#ifndef __A2E_DBG_H__
#define __A2E_DBG_H__

#include <stdint.h>

#include "a2e_def.h"

#ifndef NDEBUG
/** @brief Debug log macro for build without NDEBUG */
#define A2E_DBG(format, ...) a2e_log(format, ##__VA_ARGS__)
#else
/** @brief Debug log macro for NDEBUG build */
#define A2E_DBG(format, ...)
#endif

/** @brief Library log wrapper */
void a2e_log(const char *format, ...);

/** @brief Turn debug on/off */
void a2e_set_dbg(uint8_t on);
/** @brief Set external log function */
void a2e_set_dbg_log_func(a2e_log_func log_func);

#endif /* __A2E_DBG_H__ */

/** @} */
