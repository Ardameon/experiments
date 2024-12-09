#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

#include "a2e.h"
#include "a2e_iface.h"
#include "a2e_server.h"
#include "a2e_dbg.h"
#include "a2e_common.h"

#define RX_CHUNK_SIZE 16

static a2e_status_e server_init_func(a2e_server_t **server, const a2e_cfg_t *cfg);
static a2e_status_e server_close_func(a2e_server_t *server);

static a2e_status_e server_request_rx_func(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
static a2e_status_e server_request_complete_func(a2e_server_t *server);
static a2e_status_e server_response_tx_func(a2e_server_t *server, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);
static a2e_status_e server_progress_tx_func(a2e_server_t *server, uint16_t to_ms);

static a2e_status_e server_alloc(a2e_server_t **server);
static void         server_free(a2e_server_t *server);

static a2e_status_e server_start(a2e_server_t *server);
static a2e_status_e server_stop(a2e_server_t *server);

static a2e_status_e server_conn_accept(a2e_server_t *server, uint16_t to_ms);
static a2e_status_e server_conn_read_start(a2e_server_t *server, uint16_t to_ms);
static a2e_status_e server_conn_read(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);

static a2e_strategy_i server_iface =
{
    .init      = (a2e_init_func)             &server_init_func,
    .close     = (a2e_close_func)            &server_close_func,

    .req_rx    = (a2e_request_rx_func)       &server_request_rx_func,
    .req_cmplt = (a2e_request_complete_func) &server_request_complete_func,
    .resp_tx   = (a2e_response_tx_func)      &server_response_tx_func,
    .prog_tx   = (a2e_progress_tx_func)      &server_progress_tx_func,
};

static a2e_status_e server_init_func(a2e_server_t **server, const a2e_cfg_t *cfg)
{
    a2e_server_t *new_srv = NULL;
    a2e_status_e status = eA2E_SC_ERROR;

    a2e_dbg("%s: start", __func__);

    (*server) = NULL;

    status = server_alloc(&new_srv);
    if (status != eA2E_SC_OK)
    {
        goto _exit;
    }

    memcpy(&new_srv->base.cfg, cfg, sizeof(*cfg));
    new_srv->loc_fd = -1;
    new_srv->rem_fd = -1;
    new_srv->base.role = eA2E_ROLE_SERVER;
    new_srv->base.state = eA2E_STATE_NULL;

    status = server_start(new_srv);
    if (status != eA2E_SC_OK)
    {
        goto _exit;
    }

    (*server) = new_srv;

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_close_func(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);
    server_stop(server);
    server_free(server);
    a2e_dbg("%s: end", __func__);
    return eA2E_SC_OK;
}

static a2e_status_e server_request_rx_func(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;

    a2e_dbg("%s: start", __func__);
    switch (A2E_BASE(server)->state)
    {
        case eA2E_STATE_IDLE:
            status = server_conn_accept(server, to_ms);
            break;

        case eA2E_STATE_REQ_RX_START:
            status = server_conn_read_start(server, to_ms);
            break;

        case eA2E_STATE_REQ_RX:
            status = server_conn_read(server, rx_buffer, size, to_ms);
            break;

        default:
            status = eA2E_SC_WRONG_STATE;
            break;
    }
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_conn_accept(a2e_server_t *server, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0;
    unsigned long start_time;
    a2e_dbg("%s: start", __func__);

    start_time = a2e_get_timestamp();

    fds.fd = server->loc_fd;
    fds.events = POLLIN;

    while (1)
    {
        res = poll(&fds, 1, to_ms);
        if (res > 0)
        {
            struct sockaddr_un raddr;
            socklen_t rlen;

            if (fds.revents & POLLNVAL)
            {
                a2e_dbg("%s. Conn accept poll failed (POLLNVAL)", a2e_name(A2E_BASE(server)));
                goto _exit;
            }

            res = accept(server->loc_fd, (struct sockaddr *)&raddr, &rlen);
            if (res != -1)
            {
                server->rem_fd = res;
                fcntl(server->rem_fd, F_SETFL, O_NONBLOCK);
                a2e_set_state(A2E_BASE(server), eA2E_STATE_REQ_RX_START);
                status = eA2E_SC_CONTINUE;
            }
            else
            {
                a2e_dbg("%s. Conn accept failed (%s)", a2e_name(A2E_BASE(server)), errno);
            }

            goto _exit;
        }
        else if (res < 0)
        {
            if (errno != EINTR)
            {
                a2e_dbg("%s. Conn accept poll failed (%s)", a2e_name(A2E_BASE(server)), errno);
                goto _exit;
            }
        }
        else
        {
            status = eA2E_SC_TIMEOUT;
            goto _exit;
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_conn_read_start(a2e_server_t *server, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0;
    unsigned long start_time;
    int poll_to_ms = 50;

    a2e_dbg("%s: start", __func__);

    start_time = a2e_get_timestamp();

    fds.fd = server->rem_fd;
    fds.events = POLLIN;

    while (1)
    {
        res = poll(&fds, 1, poll_to_ms);
        if (res > 0)
        {
            if (fds.revents & (POLLNVAL | POLLERR))
            {
                a2e_dbg("%s. Conn read poll failed (POLLNVAL | PLLERR)", a2e_name(A2E_BASE(server)));
                goto _exit;
            }

            if (fds.revents & POLLHUP)
            {
                a2e_dbg("%s. Conn read connection closed by remote side", a2e_name(A2E_BASE(server)));
                goto _exit;
            }

            /* TODO: read message header here */

            server->req_size_exp = 133;
            server->req_size_recv = 0;

            server->req = malloc(server->req_size_exp);

            a2e_set_state(A2E_BASE(server), eA2E_STATE_REQ_RX);

            //rx_buf_ptr = server->req + server->req_size_recv;

            //recv_len = recv(server->rem_fd, rx_buf_ptr, RX_CHUNK_SIZE, 0);
            //if (recv_len < 0)
            //{
            //    a2e_dbg("%s. Conn read error (%s)", a2e_name(A2E_BASE(server)));
            //    goto _exit;
            //}

            status = eA2E_SC_CONTINUE;

            goto _exit;
        }
        else if (res < 0)
        {
            if (errno != EINTR)
            {
                a2e_dbg("%s. Conn read poll failed (%s)", a2e_name(A2E_BASE(server)), errno);
                goto _exit;
            }
        }
        else
        {
            a2e_dbg("%s. Conn read poll timeout (no data for read)", a2e_name(A2E_BASE(server)));
            /* cycle continue */
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}
static a2e_status_e server_conn_read(a2e_server_t *server, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0, recv_len = 0;
    unsigned long start_time;
    int poll_to_ms = 50;
    uint8_t *rx_buf_ptr;

    a2e_dbg("%s: start", __func__);

    start_time = a2e_get_timestamp();

    fds.fd = server->rem_fd;
    fds.events = POLLIN;

    while (1)
    {
        res = poll(&fds, 1, poll_to_ms);
        if (res > 0)
        {
            if (fds.revents & (POLLNVAL | POLLERR))
            {
                a2e_dbg("%s. Conn read poll failed (POLLNVAL | PLLERR)", a2e_name(A2E_BASE(server)));
                goto _exit;
            }

            if (fds.revents & POLLHUP)
            {
                a2e_dbg("%s. Conn read connection closed by remote side", a2e_name(A2E_BASE(server)));
                goto _exit;
            }

            rx_buf_ptr = server->req + server->req_size_recv;

            recv_len = recv(server->rem_fd, rx_buf_ptr, A2E_MIN(RX_CHUNK_SIZE, server->req_size_exp - server->req_size_recv), 0);
            if (recv_len < 0)
            {
                a2e_dbg("%s. Conn read error (%s)", a2e_name(A2E_BASE(server)));
                goto _exit;
            }

            server->req_size_recv += recv_len;

            a2e_dbg("%s. Recv data chunk size[%d] (total[%d/%d])", a2e_name(A2E_BASE(server)), recv_len, server->req_size_recv, server->req_size_exp);

            if (server->req_size_recv >= server->req_size_exp)
            {
                (*rx_buffer) = server->req;
                (*size) = server->req_size_recv;
                a2e_set_state(A2E_BASE(server), eA2E_STATE_REQ_RX_FINISH);
                status = eA2E_SC_OK;
                goto _exit;
            }
            else
            {
                status = eA2E_SC_CONTINUE;
                /* Cycle will be continued until read end or timeout reached */
            }
        }
        else if (res < 0)
        {
            if (errno != EINTR)
            {
                a2e_dbg("%s. Conn read poll failed (%s)", a2e_name(A2E_BASE(server)), errno);
                goto _exit;
            }
        }
        else
        {
            a2e_dbg("%s. Conn read poll timeout (no data for read)", a2e_name(A2E_BASE(server)));
            /* cycle continue */
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = (status == eA2E_SC_CONTINUE) ? eA2E_SC_CONTINUE_TIMEOUT: eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_request_complete_func(a2e_server_t *server)
{
    a2e_status_e status = eA2E_SC_WRONG_STATE;
    a2e_dbg("%s: start", __func__);

    switch(A2E_BASE(server)->state)
    {
        case eA2E_STATE_REQ_RX_START:
        case eA2E_STATE_REQ_RX:
        case eA2E_STATE_REQ_RX_FINISH:
        {
            if (server->rem_fd > -1)
            {
                shutdown(server->rem_fd, SHUT_RDWR);
                close(server->rem_fd);
                server->rem_fd = -1;
            }

            if (server->req)
            {
                free(server->req);
                server->req = 0;
                server->req_size_exp = 0;
                server->req_size_recv = 0;
            }

            a2e_set_state(A2E_BASE(server), eA2E_STATE_IDLE);
            status = eA2E_SC_OK;

            break;
        }

        default:
            break;
    }

    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}

static a2e_status_e server_response_tx_func(a2e_server_t *server, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_SC_OK;
}

static a2e_status_e server_progress_tx_func(a2e_server_t *server, uint16_t to_ms)
{
    a2e_dbg("%s: start", __func__);
    a2e_dbg("%s: end", __func__);
    return eA2E_SC_OK;
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
    a2e_status_e status = eA2E_SC_ERROR;

    a2e_dbg("%s: start", __func__);

    (*server) = NULL;

    new_srv = calloc(1, sizeof(**server));

    if (new_srv)
    {
        new_srv->iface = server_iface;

        (*server) = new_srv;

        status = eA2E_SC_OK;
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
    a2e_status_e status = eA2E_SC_ERROR;
    int reuse = 1;
    int res = -1;

    a2e_dbg("%s: start", __func__);

    a2e_dbg("%s. Server start (unix: '%s')", a2e_name(A2E_BASE(server)), A2E_BASE(server)->cfg.sock_dir);

    if (A2E_BASE(server)->state > eA2E_STATE_NULL)
    {
        status = eA2E_SC_WRONG_STATE;
        a2e_dbg("%s. Start in wrong state: %s", a2e_name(A2E_BASE(server)), a2e_state_str(A2E_BASE(server)->state));
        goto _exit;
    }

    res = stat(A2E_BASE(server)->cfg.sock_dir, &st);
    if ((res < 0) || ((st.st_mode & S_IFMT) != S_IFDIR))
    {
        a2e_dbg("%s. Socket dir '%s' is not dir or doesn't exist (%s)", a2e_name(A2E_BASE(server)), A2E_BASE(server)->cfg.sock_dir, strerror(errno));
        goto _exit;
    }

    snprintf(server->sock_path, sizeof(server->sock_path) - 1, "%s/%s", A2E_BASE(server)->cfg.sock_dir, DEF_A2E_SOCK_NAME);

    laddr.sun_family = AF_UNIX;
    strncpy(laddr.sun_path, server->sock_path, sizeof(laddr.sun_path) - 1);

    unlink(server->sock_path);

    if ((server->loc_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        a2e_dbg("%s. Failed to create socket (%s)", a2e_name(A2E_BASE(server)), strerror(errno));
        goto _exit;
    }

	setsockopt(server->loc_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if ((bind(server->loc_fd, (struct sockaddr *)&laddr, sizeof(laddr))) < 0)
	{
        a2e_dbg("%s. Failed to bind socket (%s)", a2e_name(A2E_BASE(server)), strerror(errno));
        goto _exit;
	}

    if ((listen(server->loc_fd, 1)) < 0)
    {
        a2e_dbg("%s. Failed to bind socket (%s)", a2e_name(A2E_BASE(server)), strerror(errno));
        goto _exit;
    }

    a2e_set_state(A2E_BASE(server), eA2E_STATE_IDLE);

    status = eA2E_SC_OK;

_exit:
    a2e_dbg("%s: end (%s)", __func__, a2e_perror(status));
    return status;
}


static a2e_status_e server_stop(a2e_server_t *server)
{
    a2e_dbg("%s: start", __func__);

    a2e_dbg("%s. Server stop in state %s", a2e_name(A2E_BASE(server)), a2e_state_str(A2E_BASE(server)->state));

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

    a2e_set_state(A2E_BASE(server), eA2E_STATE_NULL);

    a2e_dbg("%s: end", __func__);
    return eA2E_SC_OK;
}
