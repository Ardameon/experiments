/**
 * @file
 *
 * @addtogroup a2e_dbg
 * @{
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "a2e_dbg.h"
#include "a2e_def.h"

/** @brief Maximum log string length */
#define LOG_STR_LEN_MAX 1024

/** @brief Library debug cfg structure */
typedef struct a2e_dbg_cfg_t
{
    /** External debug log function */
    a2e_log_func log_func;
    /** Debug enabled flag */
    uint8_t dbg_on;
} a2e_dbg_cfg_t;

/** @brief Debug config */
static a2e_dbg_cfg_t dbg_cfg;

/** @brief Debug log buffer */
static char log_buf[LOG_STR_LEN_MAX];

/**
 * @copybrief a2e_log
 *
 * This function will use external debug log function if it is set and printf() in other case
 *
 * @param[in]  format  Debug log formated string
 */
void a2e_log(const char *format, ...)
{
    if (dbg_cfg.dbg_on)
    {
        va_list args;
        int len = 0;
        va_start(args, format);
        len = vsnprintf(log_buf, sizeof(log_buf), format, args);
        va_end(args);

        if (len < 0)
            goto _exit;

        if (len >= sizeof(log_buf))
            len = sizeof(log_buf) - 1;

        if (dbg_cfg.log_func)
        {
            dbg_cfg.log_func(log_buf, len);
        }
        else
        {
            printf("[A2E DBG] %.*s\n", len, log_buf);
        }
    }

_exit:
    return;
}

/**
 * @copybrief a2e_set_dbg
 *
 * @param[in]  on  Logs on/off
 */
void a2e_set_dbg(uint8_t on)
{
    dbg_cfg.dbg_on = !!on;
}

/**
 * @copybrief a2e_set_dbg
 *
 * @param[in]  log_func  Pointer to external debug log function
 */
void a2e_set_dbg_log_func(a2e_log_func log_func)
{
    dbg_cfg.log_func = log_func;
}

/** @} */

