#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "a2e_dbg.h"
#include "a2e_def.h"

#define LOG_STR_LEN_MAX 1024

typedef struct a2e_dbg_cfg_t
{
    a2e_log_func log_func;
    uint8_t dbg_on;
} a2e_dbg_cfg_t;

static a2e_dbg_cfg_t dbg_cfg;

char log_buf[LOG_STR_LEN_MAX + 1];

void a2e_log(const char *format, ...)
{
    if (dbg_cfg.dbg_on)
    {
        va_list args;
        int len = 0;
        va_start(args, format);
        len = vsnprintf(log_buf, LOG_STR_LEN_MAX, format, args);
        va_end(args);

        if (len < 0)
            goto _exit;

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

void a2e_set_dbg(uint8_t on)
{
    dbg_cfg.dbg_on = !!on;
}

void a2e_set_dbg_log_func(a2e_log_func log_func)
{
    dbg_cfg.log_func = log_func;
}


