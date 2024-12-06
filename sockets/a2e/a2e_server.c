#include <stdlib.h>
#include <string.h>

#include "a2e_iface.h"
#include "a2e_server.h"
#include "a2e_dbg.h"

static a2e_status_e server_init(a2e_server_t **server, const a2e_cfg_t *cfg);
static a2e_status_e server_close(a2e_server_t *server);

static a2e_status_e server_request_rx(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size);
static a2e_status_e server_request_complete(a2e_server_t *server);
static a2e_status_e server_response_tx(a2e_server_t *server, uint8_t *tx_buffer, uint32_t size);
static a2e_status_e server_progress_tx(a2e_server_t *server);

static a2e_strategy_i server_iface =
{
    .init      = (a2e_init_func)             &server_init,
    .close     = (a2e_close_func)            &server_close,

    .req_rx    = (a2e_request_rx_func)       &server_request_rx,
    .req_cmplt = (a2e_request_complete_func) &server_request_complete,
    .resp_tx   = (a2e_response_tx_func)      &server_response_tx,
    .prog_tx   = (a2e_progress_tx_func)      &server_progress_tx,
};

static a2e_status_e server_init(a2e_server_t **server, const a2e_cfg_t *cfg)
{
    a2e_server_t *new_srv = NULL;
    a2e_status_e status = eA2E_STATUS_ERROR;

    a2e_dbg("%s: start", __func__);

    (*server) = NULL;

    new_srv = calloc(1, sizeof(**server));

    if (new_srv)
    {
        new_srv->iface = server_iface;
        memcpy(&new_srv->base.cfg, cfg, sizeof(*cfg));

        (*server) = new_srv;

        status = eA2E_STATUS_OK;
    }

    a2e_dbg("%s: end", __func__);

    return status;
}

static a2e_status_e server_close(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);
    free(server);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e server_request_rx(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e server_request_complete(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e server_response_tx(a2e_server_t *server, uint8_t *tx_buffer, uint32_t size)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e server_progress_tx(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

a2e_strategy_i a2e_server_iface_get(void)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return server_iface;
}

