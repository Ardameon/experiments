#include "a2e.h"
#include "a2e_iface.h"
#include "a2e_client.h"
#include "a2e_server.h"
#include "a2e_dbg.h"

typedef struct a2e_ext_t
{
    a2e_t base;
    a2e_strategy_i iface;
} a2e_ext_t;


a2e_status_e a2e_init_client(a2e_t **a2e)
{
    a2e_dbg("%s: start", __func__);
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_strategy_i client_iface = a2e_client_iface_get();

    if (client_iface.init)
    {
        status = client_iface.init(a2e);
    }

    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_init_server(a2e_t **a2e)
{
    a2e_dbg("%s: start", __func__);
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_strategy_i server_iface = a2e_server_iface_get();

    if (server_iface.init)
    {
        status = server_iface.init(a2e);
    }

    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_request_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !rx_buffer || !size)
    {
        status = eA2E_STATUS_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_rx)
    {
        status = ext->iface.req_rx(a2e, rx_buffer, size);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_request_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !tx_buffer)
    {
        status = eA2E_STATUS_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_tx)
    {
        status = ext->iface.req_tx(a2e, tx_buffer, size);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_request_complete(a2e_t *a2e)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_STATUS_INVALID_ARGS;
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


a2e_status_e a2e_response_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !rx_buffer || !size)
    {
        status = eA2E_STATUS_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.resp_rx)
    {
        status = ext->iface.resp_rx(a2e, rx_buffer, size);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


a2e_status_e a2e_response_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e || !tx_buffer)
    {
        status = eA2E_STATUS_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.resp_tx)
    {
        status = ext->iface.resp_tx(a2e, tx_buffer, size);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}



a2e_status_e a2e_progress_tx(a2e_t *a2e)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_STATUS_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.prog_tx)
    {
        status = ext->iface.prog_tx(a2e);
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;

}


a2e_status_e a2e_close(a2e_t *a2e)
{
    a2e_status_e status = eA2E_STATUS_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    a2e_dbg("%s: start", __func__);

    if (!a2e)
    {
        status = eA2E_STATUS_INVALID_ARGS;
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

void a2e_set_dbg_on(void)
{
    a2e_set_dbg(1);
}

void a2e_set_dbg_off(void)
{
    a2e_set_dbg(0);
}

void a2e_set_dbg_func(a2e_log_func log_func)
{
    a2e_set_dbg_log_func(log_func);
}

const char *a2e_perror(a2e_status_e status)
{
    switch(status)
    {
        case eA2E_STATUS_OK:              return "OK";
        case eA2E_STATUS_NOT_IMPLEMENTED: return "NOT_IMPLEMENTED";
        case eA2E_STATUS_INVALID_ARGS:    return "INVALID_ARGS";
        case eA2E_STATUS_ERROR:           return "ERROR";
        default:                          return "UNDEF_STATUS";
    }
}
