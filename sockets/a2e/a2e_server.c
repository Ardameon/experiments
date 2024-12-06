#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "a2e.h"
#include "a2e_iface.h"
#include "a2e_server.h"
#include "a2e_dbg.h"
#include "a2e_common.h"

static a2e_status_e server_init(a2e_server_t **server, const a2e_cfg_t *cfg);
static a2e_status_e server_close(a2e_server_t *server);

static a2e_status_e server_request_rx(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size);
static a2e_status_e server_request_complete(a2e_server_t *server);
static a2e_status_e server_response_tx(a2e_server_t *server, uint8_t *tx_buffer, uint32_t size);
static a2e_status_e server_progress_tx(a2e_server_t *server);

static a2e_status_e server_alloc(a2e_server_t **server);
static void         server_free(a2e_server_t *server);

static a2e_status_e server_start(a2e_server_t *server);
static a2e_status_e server_stop(a2e_server_t *server);

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

    status = server_alloc(&new_srv);
    if (status != eA2E_STATUS_OK)
    {
        goto _exit;
    }

    memcpy(&new_srv->base.cfg, cfg, sizeof(*cfg));
    new_srv->loc_fd = -1;
    new_srv->rem_fd = -1;

    status = server_start(new_srv);
    if (status != eA2E_STATUS_OK)
    {
        goto _exit;
    }

    (*server) = new_srv;

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_close(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);
    server_stop(server);
    server_free(server);
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

static a2e_status_e server_alloc(a2e_server_t **server)
{
    a2e_server_t *new_srv = NULL;
    a2e_status_e status = eA2E_STATUS_ERROR;

    a2e_dbg("%s: start", __func__);

    (*server) = NULL;

    new_srv = calloc(1, sizeof(**server));

    if (new_srv)
    {
        new_srv->iface = server_iface;

        (*server) = new_srv;

        status = eA2E_STATUS_OK;
    }

    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static void server_free(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);
    free(server);
    a2e_dbg("%s: end", __func__);
}


static a2e_status_e server_start(a2e_server_t *server)
{
    struct stat st = {0};
    struct sockaddr_un laddr = {0};
    a2e_status_e status = eA2E_STATUS_ERROR;
    int reuse = 1;
    int res = -1;

    a2e_dbg("%s: start", __func__);

    a2e_dbg("%s. Server start (unix: '%s')", a2e_name(&server->base), server->base.cfg.sock_dir);

    if (server->base.state > eA2E_STATE_NULL)
    {
        status = eA2E_STATUS_WRONG_STATE;
        a2e_dbg("%s. Start in wrong state: %s", a2e_name(&server->base), a2e_state_str(server->base.state));
        goto _exit;
    }

    res = stat(server->base.cfg.sock_dir, &st);
    if ((res < 0) || ((st.st_mode & S_IFMT) != S_IFDIR))
    {
        a2e_dbg("%s. Socket dir '%s' is not dir or doesn't exist (%s)", a2e_name(&server->base), server->base.cfg.sock_dir, strerror(errno));
        goto _exit;
    }

    snprintf(server->sock_path, sizeof(server->sock_path) - 1, "%s/%s", server->base.cfg.sock_dir, DEF_A2E_SOCK_NAME);

    laddr.sun_family = AF_UNIX;
    strncpy(laddr.sun_path, server->sock_path, sizeof(laddr.sun_path) - 1);

    unlink(server->sock_path);

    if ((server->loc_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        a2e_dbg("%s. Failed to create socket (%s)", a2e_name(&server->base), strerror(errno));
        goto _exit;
    }

	setsockopt(server->loc_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if ((bind(server->loc_fd, (struct sockaddr *)&laddr, sizeof(laddr))) < 0)
	{
        a2e_dbg("%s. Failed to bind socket (%s)", a2e_name(&server->base), strerror(errno));
        goto _exit;
	}

    if ((listen(server->loc_fd, 1)) < 0)
    {
        a2e_dbg("%s. Failed to bind socket (%s)", a2e_name(&server->base), strerror(errno));
        goto _exit;
    }

    a2e_set_state(&server->base, eA2E_STATE_IDLE);

    status = eA2E_STATUS_OK;

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_stop(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);

    a2e_dbg("%s. Server stop in state %s", a2e_name(&server->base), a2e_state_str(server->base.state));

    if (server->loc_fd > -1)
    {
        shutdown(server->loc_fd, SHUT_RDWR);
        close(server->loc_fd);
    }

    if (server->rem_fd > -1)
    {
        shutdown(server->rem_fd, SHUT_RDWR);
        close(server->rem_fd);
    }

    unlink(server->sock_path);

    a2e_set_state(&server->base, eA2E_STATE_NULL);

    a2e_dbg("%s: end", __func__);
    return eA2E_STATUS_OK;
}
