#include <stdlib.h>
#include <string.h>

#include "a2e_iface.h"
#include "a2e_client.h"
#include "a2e_dbg.h"

static a2e_status_e client_init(a2e_client_t **client, const a2e_cfg_t *cfg);
static a2e_status_e client_close(a2e_client_t *client);

static a2e_status_e client_request_tx(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size);
static a2e_status_e client_request_complete(a2e_client_t *client);
static a2e_status_e client_response_rx(a2e_client_t *client, uint8_t **rx_buffer, uint32_t *size);

static a2e_strategy_i client_iface =
{
    .init      = (a2e_init_func)             &client_init,
    .close     = (a2e_close_func)            &client_close,

    .req_tx    = (a2e_request_tx_func)       &client_request_tx,
    .req_cmplt = (a2e_request_complete_func) &client_request_complete,
    .resp_rx   = (a2e_response_rx_func)      &client_response_rx,
};

static a2e_status_e client_init(a2e_client_t **client, const a2e_cfg_t *cfg)
{
    a2e_client_t *new_clt = NULL;
    a2e_status_e status = eA2E_STATUS_ERROR;

    a2e_dbg("%s: start", __func__);

    (*client) = NULL;

    new_clt = calloc(1, sizeof(**client));

    if (new_clt)
    {
        new_clt->iface = client_iface;
        memcpy(&new_clt->base.cfg, cfg, sizeof(*cfg));

        (*client) = new_clt;

        status = eA2E_STATUS_OK;
    }

    a2e_dbg("%s: end", __func__);

    return status;
}

static a2e_status_e client_close(a2e_client_t *client)
{
    a2e_dbg("%s: start", __func__);
    free(client);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e client_request_tx(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e client_request_complete(a2e_client_t *client)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

static a2e_status_e client_response_rx(a2e_client_t *client, uint8_t **rx_buffer, uint32_t *size)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}

a2e_strategy_i a2e_client_iface_get(void)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return client_iface;
}
