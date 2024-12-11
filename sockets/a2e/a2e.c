#include <string.h>

#include "a2e.h"
#include "a2e_iface.h"
#include "a2e_client.h"
#include "a2e_server.h"
#include "a2e_dbg.h"
#include "a2e_common.h"

typedef struct a2e_ext_t
{
    a2e_t base;
    a2e_strategy_i iface;
} a2e_ext_t;


a2e_status_e a2e_init_client(a2e_t **a2e, const a2e_cfg_t *cfg)
{
    a2e_dbg("%s: start", __func__);
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_strategy_i client_iface = a2e_client_iface_get();

    if (client_iface.init)
    {
        status = client_iface.init(a2e, cfg);
    }

    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_init_server(a2e_t **a2e, const a2e_cfg_t *cfg)
{
    a2e_dbg("%s: start", __func__);
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_strategy_i server_iface = a2e_server_iface_get();

    if (server_iface.init)
    {
        status = server_iface.init(a2e, cfg);
    }

    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_request_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !rx_buffer || !size)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_rx)
    {
        status = ext->iface.req_rx(a2e, rx_buffer, size, to_ms);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_request_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !tx_buffer)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_tx)
    {
        status = ext->iface.req_tx(a2e, tx_buffer, size, to_ms);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_request_complete(a2e_t *a2e)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_cmplt)
    {
        status = ext->iface.req_cmplt(a2e);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

a2e_status_e a2e_request_complete_wait(a2e_t *a2e, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_cmplt_wait)
    {
        status = ext->iface.req_cmplt_wait(a2e, to_ms);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_response_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !rx_buffer || !size)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.resp_rx)
    {
        status = ext->iface.resp_rx(a2e, rx_buffer, size, to_ms);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_response_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !tx_buffer)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.resp_tx)
    {
        status = ext->iface.resp_tx(a2e, tx_buffer, size, to_ms);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}



a2e_status_e a2e_progress_tx(a2e_t *a2e, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.prog_tx)
    {
        status = ext->iface.prog_tx(a2e, to_ms);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_close(a2e_t *a2e)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.close)
    {
        status = ext->iface.close(a2e);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;

}

void a2e_dbg_on(void)
{
    a2e_set_dbg(1);
}

void a2e_dbg_off(void)
{
    a2e_set_dbg(0);
}

void a2e_dbg_set_func(a2e_log_func log_func)
{
    a2e_set_dbg_log_func(log_func);
}

const char *a2e_perror(a2e_status_e status)
{
    switch(status)
    {
        case eA2E_SC_OK:               return "OK";
        case eA2E_SC_IN_PROGRESS:      return "IN_PROGRESS";
        case eA2E_SC_TIMEOUT:          return "TIMEOUT";
        case eA2E_SC_NOT_IMPLEMENTED:  return "NOT_IMPLEMENTED";
        case eA2E_SC_INVALID_ARGS:     return "INVALID_ARGS";
        case eA2E_SC_NO_MEM:           return "NO_MEM";
        case eA2E_SC_WRONG_STATE:      return "WRONG_STATE";
        case eA2E_SC_INCOMPLETE_READ:  return "INCOMPLETE_READ";
        case eA2E_SC_CONTINUE:         return "CONTINUE";
        case eA2E_SC_CONTINUE_TIMEOUT: return "CONTINUE_TIMEOUT";
        case eA2E_SC_ERROR:            return "ERROR";
        default:                       return "UNDEF_STATUS";
    }
}

void a2e_cfg_set_default(a2e_cfg_t *cfg)
{
    if (cfg)
    {
        strncpy(cfg->sock_dir, DEF_A2E_SOCK_DIR, A2E_SOCK_DIR_LEN_MAX);
        strncpy(cfg->name, DEF_A2E_NAME, A2E_NAME_LEN_MAX);
        cfg->rw_chunk_size = DEF_A2E_RW_CHUNK_SIZE;
        cfg->req_cmplt_wait_tries = DEF_A2E_REQ_CMPLT_WAIT_TRIES;
    }
}
