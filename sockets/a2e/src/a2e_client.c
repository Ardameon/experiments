/**
 * @file
 *
 * @addtogroup a2e_client
 *
 * @{
 */

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
#include "a2e_client.h"
#include "a2e_dbg.h"
#include "a2e_common.h"
#include "a2e_msg.h"

/**
 * @name Private functions
 * @{
 */

/** @brief A2E-client-instance init */
static a2e_status_e client_init_func(a2e_client_t **client, const a2e_cfg_t *cfg);
/** @brief A2E-client-instance close */
static a2e_status_e client_close_func(a2e_client_t *client);

/** @brief A2E-client-instance request TX */
static a2e_status_e client_request_tx_func(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);
/** @brief A2E-client-instance request complete */
static a2e_status_e client_request_complete_func(a2e_client_t *client);
/** @brief A2E-client-instance response RX */
static a2e_status_e client_response_rx_func(a2e_client_t *client, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);

/** @brief A2E-client-instance alloc */
static a2e_status_e client_alloc(a2e_client_t **client);
/** @brief A2E-client-instance free */
static void         client_free(a2e_client_t *client);

/** @brief A2E-client-instance start */
static a2e_status_e client_start(a2e_client_t *client);
/** @brief A2E-client-instance stop */
static a2e_status_e client_stop(a2e_client_t *client);

/** @brief A2E-client-instance connect to peer (server) */
static a2e_status_e client_conn_connect(a2e_client_t *client);
/** @brief A2E-client-instance start read data from fd (read received @ref a2e_msg_t "message header" from peer) */
static a2e_status_e client_conn_read_start(a2e_client_t *client, uint16_t to_ms);
/** @brief A2E-client-instance read data from fd (read opaque data received from peer) */
static a2e_status_e client_conn_read(a2e_client_t *client, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
/** @brief A2E-client-instance start write data to fd (send @ref a2e_msg_t "message header" to peer) */
static a2e_status_e client_conn_write_start(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);
/** @brief A2E-client-instance write data to fd (send opaque data to peer) */
static a2e_status_e client_conn_write(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);

/** @} */

/** @brief A2E-client-instance interface object */
static a2e_strategy_i client_iface =
{
     (a2e_init_func)             &client_init_func,              /* init           */
     (a2e_close_func)            &client_close_func,             /* close          */
                                 NULL,                           /* req_rx         */
     (a2e_request_tx_func)       &client_request_tx_func,        /* req_tx         */
     (a2e_request_complete_func) &client_request_complete_func,  /* req_cmplt      */
                                 NULL,                           /* req_cmplt_wait */
     (a2e_response_rx_func)      &client_response_rx_func,       /* resp_rx        */
                                 NULL,                           /* resp_tx        */
                                 NULL,                           /* prog_tx        */
};

/**
 * @copybrief client_init_func
 *
 * @param[out]  client  Pointer to newly allocated and initialized UNIX stream socket client
 * @param[in]   cfg     Configuration for new client
 *
 * @return @ref eA2E_SC_OK on success
 */
static a2e_status_e client_init_func(a2e_client_t **client, const a2e_cfg_t *cfg)
{
    a2e_client_t *new_clt = NULL;
    a2e_status_e status = eA2E_SC_ERROR;

    if (!client || !cfg)
        goto _exit;

    (*client) = NULL;

    status = client_alloc(&new_clt);
    if (status != eA2E_SC_OK)
    {
        goto _exit;
    }

    a2e_cfg_apply(A2E_BASE(new_clt), cfg);

    new_clt->fd = -1;
    new_clt->base.role = eA2E_ROLE_CLIENT;
    new_clt->base.state = eA2E_STATE_NULL;

    status = client_start(new_clt);
    if (status != eA2E_SC_OK)
    {
        goto _exit;
    }

    (*client) = new_clt;

_exit:
    return status;
}

/**
 * @copybrief client_close_func
 *
 * @param[in]   client  UNIX stream socket client to close
 *
 * @return @ref eA2E_SC_OK on success
 */
static a2e_status_e client_close_func(a2e_client_t *client)
{
    if (client)
    {
        client_stop(client);
        client_free(client);
    }

    return eA2E_SC_OK;
}

/**
 * @copybrief client_request_tx_func
 *
 * @param[in]   client     Client
 * @param[in]   tx_buffer  Pointer to buffer with request to send
 * @param[in]   size       Buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * @return @ref eA2E_SC_OK on successful request sending
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue sending session)
 *        @n @ref eA2E_SC_CONTINUE on successful connecting to peer and sending information about upcoming request
 *        @n @ref eA2E_SC_CONTINUE_TIMEOUT on function timeout acquired during active sending session
 *        @n Other status codes should be interpreted as errors
 */
static a2e_status_e client_request_tx_func(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;

    if (!client)
        goto _exit;

    switch (A2E_BASE(client)->state)
    {
        case eA2E_STATE_IDLE:
            status = client_conn_connect(client);
            break;

        case eA2E_STATE_REQ_TX_START:
            status = client_conn_write_start(client, tx_buffer, size, to_ms);
            break;

        case eA2E_STATE_REQ_TX:
            status = client_conn_write(client, tx_buffer, size, to_ms);
            break;

        default:
            status = eA2E_SC_WRONG_STATE;
            break;
    }

_exit:
    return status;
}

/**
 * @copybrief client_request_complete_func
 *
 * @param[in]   client      Client
 *
 * @return @ref eA2E_SC_OK on success
 */
static a2e_status_e client_request_complete_func(a2e_client_t *client)
{
    a2e_status_e status = eA2E_SC_ERROR;

    if (!client)
        goto _exit;

    switch(A2E_BASE(client)->state)
    {
        case eA2E_STATE_RSP_RX:
        case eA2E_STATE_RSP_RX_FINISH:
        case eA2E_STATE_REQ_PROGRESS:
        case eA2E_STATE_REQ_TX:
        case eA2E_STATE_REQ_TX_FINISH:
        {
            if (client->fd > -1)
            {
                shutdown(client->fd, SHUT_RDWR);
                close(client->fd);
                client->fd = -1;
            }

            if (client->rsp)
            {
                free(client->rsp);
                client->rsp = NULL;
                client->rsp_size_exp = 0;
                client->rsp_size_recv = 0;
            }

            client->req_size_sent = 0;

            a2e_set_state(A2E_BASE(client), eA2E_STATE_IDLE);
            status = eA2E_SC_OK;

            break;
        }

        default:
            status = eA2E_SC_WRONG_STATE;
            break;
    }

_exit:
    return status;
}

/**
 * @copybrief client_response_rx_func
 *
 * @param[in]   client     Client
 * @param[out]  rx_buffer  Pointer to buffer with received response
 * @param[out]  size       Received buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * @return @ref eA2E_SC_OK on successful response receiving, after that we can handle rx_buffer
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue receiving session)
 *        @n @ref eA2E_SC_CONTINUE on successful peer connecting and receiving information about upcoming request
 *        @n @ref eA2E_SC_CONTINUE_TIMEOUT on function timeout acquired during active receiving session
 *        @n Other status codes should be interpreted as errors
 */
static a2e_status_e client_response_rx_func(a2e_client_t *client, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;

    if (!client)
        goto _exit;

    switch (A2E_BASE(client)->state)
    {
        case eA2E_STATE_REQ_TX_FINISH:
        case eA2E_STATE_REQ_PROGRESS:
            status = client_conn_read_start(client, to_ms);
            break;

        case eA2E_STATE_RSP_RX:
            status = client_conn_read(client, rx_buffer, size, to_ms);
            break;

        default:
            status = eA2E_SC_WRONG_STATE;
            break;
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_client_iface_get
 *
 * @return UNIX stream socket client function interface object
 */
a2e_strategy_i a2e_client_iface_get(void)
{
    return client_iface;
}

#ifndef A2E_UTEST_MEM_MOCK
/**
 * @copybrief client_alloc
 *
 * @param[out]   client     Allocated client
 *
 * @return @ref eA2E_SC_OK on success
 */
static a2e_status_e client_alloc(a2e_client_t **client)
{
    a2e_client_t *new_clt = NULL;
    a2e_status_e status = eA2E_SC_NO_MEM;

    (*client) = NULL;

    new_clt = (a2e_client_t *)calloc(1, sizeof(**client));

    if (new_clt)
    {
        new_clt->iface = client_iface;

        (*client) = new_clt;

        status = eA2E_SC_OK;
    }

    return status;
}
#endif

#ifndef A2E_UTEST_MEM_MOCK
/**
 * @copybrief client_free
 *
 * @param[in]   client     Client to free
 */
static void client_free(a2e_client_t *client)
{
    free(client);
}
#endif


/**
 * @copybrief client_start
 *
 * @param[in]   client     Client to start
 *
 * @return @ref eA2E_SC_OK on success
 */
static a2e_status_e client_start(a2e_client_t *client)
{
    struct stat st = {0};
    a2e_status_e status = eA2E_SC_ERROR;
    int res = -1;

    if (!client)
        goto _exit;

    a2e_log("%s. Client start (unix: '%s')", a2e_name(A2E_BASE(client)), A2E_BASE(client)->cfg.sock_dir);

    if (A2E_BASE(client)->state > eA2E_STATE_NULL)
    {
        status = eA2E_SC_WRONG_STATE;
        a2e_log("%s. Start in wrong state: %s", a2e_name(A2E_BASE(client)), a2e_state_str(A2E_BASE(client)->state));
        goto _exit;
    }

    res = stat(A2E_BASE(client)->cfg.sock_dir, &st);
    if ((res < 0) || ((st.st_mode & S_IFMT) != S_IFDIR))
    {
        a2e_log("%s. Socket dir '%s' is not dir or doesn't exist (%s)", a2e_name(A2E_BASE(client)), A2E_BASE(client)->cfg.sock_dir, strerror(errno));
        goto _exit;
    }

    snprintf(client->sock_path, sizeof(client->sock_path), "%s/%s", A2E_BASE(client)->cfg.sock_dir, DEF_A2E_SOCK_NAME);

    a2e_set_state(A2E_BASE(client), eA2E_STATE_IDLE);

    status = eA2E_SC_OK;

_exit:
    return status;
}

/**
 * @copybrief client_stop
 *
 * @param[in]   client     Client to stop
 *
 * @return @ref eA2E_SC_OK on success
 */
static a2e_status_e client_stop(a2e_client_t *client)
{
    a2e_log("%s. Client stop in state %s", a2e_name(A2E_BASE(client)), a2e_state_str(A2E_BASE(client)->state));

    if (client->fd > -1)
    {
        shutdown(client->fd, SHUT_RDWR);
        close(client->fd);
    }

    a2e_set_state(A2E_BASE(client), eA2E_STATE_NULL);

    return eA2E_SC_OK;
}

/**
 * @copybrief client_conn_connect
 *
 * @param[in]   client     Client
 *
 * @return @ref eA2E_SC_CONTINUE on success
 */
static a2e_status_e client_conn_connect(a2e_client_t *client)
{
    a2e_status_e status = eA2E_SC_ERROR;
    int res = 0;
    struct sockaddr_un raddr = {0};
    int reuse = 1;

    if (!client)
        goto _exit;

    if ((client->fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        a2e_log("%s. Failed to create socket (%s)", a2e_name(A2E_BASE(client)), strerror(errno));
        goto _exit;
    }

    setsockopt(client->fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    fcntl(client->fd, F_SETFL, O_NONBLOCK);

    raddr.sun_family = AF_UNIX;
    strncpy(raddr.sun_path, client->sock_path, sizeof(raddr.sun_path));

    res = connect(client->fd, (struct sockaddr *)&raddr, sizeof(raddr));
    if (res != -1)
    {
        a2e_set_state(A2E_BASE(client), eA2E_STATE_REQ_TX_START);
        status = eA2E_SC_CONTINUE;
    }
    else
    {
        a2e_log("%s. Conn connect failed (%s)", a2e_name(A2E_BASE(client)), strerror(errno));
    }

_exit:
    return status;
}

/**
 * @copybrief client_conn_read_start
 *
 * @param[in]   client     Client
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * Try to receive @ref a2e_msg_t "response message info" from peer
 *
 * @return @ref eA2E_SC_CONTINUE on successful response message info receive
 *        @n @ref eA2E_SC_IN_PROGRESS on successful progress message info receive
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue receiving session, no data on socket)
 *        @n Other status codes should be interpreted as errors
 */
static a2e_status_e client_conn_read_start(a2e_client_t *client, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0, recv_len = 0;
    unsigned long start_time;
    int poll_to_ms = DEF_A2E_RW_POLL_TIMEOUT_MS;
    a2e_msg_t msg;

    if (!client)
        goto _exit;

    start_time = a2e_get_timestamp();

    fds.fd = client->fd;
    fds.events = POLLIN;

    while(1)
    {
        status = eA2E_SC_ERROR;

        res = poll(&fds, 1, poll_to_ms);
        if (res > 0)
        {
            if (fds.revents & (POLLNVAL | POLLERR))
            {
                a2e_log("%s. Conn read poll failed (POLLNVAL | PLLERR)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (fds.revents & POLLHUP)
            {
                a2e_log("%s. Conn read connection closed by remote side", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (!(fds.revents & POLLIN))
            {
                a2e_log("%s. Conn read no input data", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            recv_len = recv(client->fd, &msg, sizeof(msg), 0);
            if (recv_len < 0)
            {
                a2e_log("%s. Conn read error (%s)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }
            else if (recv_len < sizeof(msg) || msg.magic != A2E_MSG_MAGIC)
            {
                a2e_log("%s. Conn read malformed msg received", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (msg.type == eA2E_MSG_PROGRESS)
            {
                a2e_log("%s. RX Progress msg", a2e_name(A2E_BASE(client)));
                a2e_set_state(A2E_BASE(client), eA2E_STATE_REQ_PROGRESS);
                status = eA2E_SC_IN_PROGRESS;
                goto _exit;
            }

            client->rsp_size_exp = msg.len;
            client->rsp_size_recv = 0;

#ifndef A2E_UTEST_MEM_MOCK
            client->rsp = (uint8_t  *)malloc(client->rsp_size_exp);
#endif
            if (!client->rsp)
            {
                status = eA2E_SC_NO_MEM;
                goto _exit;
            }

            a2e_log("%s. RX Response msg (size: %u)", a2e_name(A2E_BASE(client)), client->rsp_size_exp);

            a2e_set_state(A2E_BASE(client), eA2E_STATE_RSP_RX);

            status = eA2E_SC_CONTINUE;

            goto _exit;
        }
        else if (res < 0)
        {
            if (errno != EINTR)
            {
                a2e_log("%s. Conn read poll failed (%s)", a2e_name(A2E_BASE(client)), strerror(errno));
                goto _exit;
            }
        }
        else
        {
            A2E_DBG("%s. Conn read poll timeout (no data for read)", a2e_name(A2E_BASE(client)));
            /* cycle continue */
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    return status;
}

/**
 * @copybrief client_conn_read
 *
 * @param[in]   client     Client
 * @param[out]  rx_buffer  Pointer to buffer with received request
 * @param[out]  size       Received buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * Try to receive response message itself
 *
 * @return @ref eA2E_SC_OK on successful and full response receiving
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue receiving session)
 *        @n @ref eA2E_SC_CONTINUE_TIMEOUT on function timeout acquired during active receiving session
 *        @n Other status codes should be interpreted as errors
 */
static a2e_status_e client_conn_read(a2e_client_t *client, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0, recv_len = 0;
    unsigned long start_time;
    int poll_to_ms = DEF_A2E_RW_POLL_TIMEOUT_MS;
    uint8_t *rx_buf_ptr;

    if (!client || !rx_buffer || !size)
        goto _exit;

    start_time = a2e_get_timestamp();

    fds.fd = client->fd;
    fds.events = POLLIN;

    while(1)
    {
        status = eA2E_SC_ERROR;

        res = poll(&fds, 1, poll_to_ms);
        if (res > 0)
        {
            if (fds.revents & (POLLNVAL | POLLERR))
            {
                a2e_log("%s. Conn read poll failed (POLLNVAL | PLLERR)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (fds.revents & POLLHUP)
            {
                a2e_log("%s. Conn read connection closed by remote side", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (!(fds.revents & POLLIN))
            {
                a2e_log("%s. Conn read no input data", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            rx_buf_ptr = client->rsp + client->rsp_size_recv;

            recv_len = recv(client->fd, rx_buf_ptr, A2E_MIN(A2E_BASE(client)->cfg.rw_chunk_size, client->rsp_size_exp - client->rsp_size_recv), 0);
            if (recv_len < 0)
            {
                a2e_log("%s. Conn read error (%s)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            client->rsp_size_recv += recv_len;

            a2e_log("%s. Recv data chunk size[%d] (total: %d/%d)", a2e_name(A2E_BASE(client)), recv_len, client->rsp_size_recv, client->rsp_size_exp);

            if (client->rsp_size_recv >= client->rsp_size_exp)
            {
                (*rx_buffer) = client->rsp;
                (*size) = client->rsp_size_recv;
                a2e_set_state(A2E_BASE(client), eA2E_STATE_RSP_RX_FINISH);
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
                a2e_log("%s. Conn read poll failed (%s)", a2e_name(A2E_BASE(client)), strerror(errno));
                goto _exit;
            }
        }
        else
        {
            A2E_DBG("%s. Conn read poll timeout (no data for read)", a2e_name(A2E_BASE(client)));
            /* cycle continue */
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = (status == eA2E_SC_CONTINUE) ? eA2E_SC_CONTINUE_TIMEOUT: eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    return status;
}

/**
 * @copybrief client_conn_write_start
 *
 * @param[in]   client     Client
 * @param[in]   tx_buffer  Pointer to buffer with request to send
 * @param[in]   size       Buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * Try to send @ref a2e_msg_t "request message info" to peer
 *
 * @return @ref eA2E_SC_CONTINUE on successful request message info send
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue sending session, socket is not ready for write)
 *        @n Other status codes should be interpreted as errors
 */
static a2e_status_e client_conn_write_start(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0, sent_len = 0;
    unsigned long start_time;
    int poll_to_ms = DEF_A2E_RW_POLL_TIMEOUT_MS;
    a2e_msg_t msg;

    if (!client)
        goto _exit;

    start_time = a2e_get_timestamp();

    fds.fd = client->fd;
    fds.events = POLLOUT;

    while(1)
    {
        status = eA2E_SC_ERROR;

        res = poll(&fds, 1, poll_to_ms);
        if (res > 0)
        {
            if (fds.revents & (POLLNVAL | POLLERR))
            {
                a2e_log("%s. Conn write poll failed (POLLNVAL | PLLERR)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (fds.revents & POLLHUP)
            {
                a2e_log("%s. Conn write connection closed by remote side", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (!(fds.revents & POLLOUT))
            {
                a2e_log("%s. Conn write output error", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            a2e_msg_fill_req(&msg, size);

            a2e_log("%s. TX Request msg (size: %u)", a2e_name(A2E_BASE(client)), size);

            sent_len = send(client->fd, &msg, sizeof(msg), 0);
            if (sent_len < 0)
            {
                a2e_log("%s. Conn write msg error (%s)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            client->req_size_sent = 0;

            a2e_set_state(A2E_BASE(client), eA2E_STATE_REQ_TX);

            status = eA2E_SC_CONTINUE;

            goto _exit;
        }
        else if (res < 0)
        {
            if (errno != EINTR)
            {
                a2e_log("%s. Conn write poll failed (%s)", a2e_name(A2E_BASE(client)), strerror(errno));
                goto _exit;
            }
        }
        else
        {
            A2E_DBG("%s. Conn write poll timeout (write is not possible)", a2e_name(A2E_BASE(client)));
            /* cycle continue */
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    return status;
}

/**
 * @copybrief client_conn_write
 *
 * @param[in]   client     Client
 * @param[in]   tx_buffer  Pointer to buffer with request to send
 * @param[in]   size       Buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * Try to send request message itself
 *
 * @return @ref eA2E_SC_OK on successful and full request send
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue sending session)
 *        @n @ref eA2E_SC_CONTINUE_TIMEOUT on function timeout acquired during active sending session
 *        @n Other status codes should be interpreted as errors
 */
static a2e_status_e client_conn_write(a2e_client_t *client, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_ERROR;
    struct pollfd fds = {0};
    int res = 0, sent_len = 0;
    unsigned long start_time;
    int poll_to_ms = DEF_A2E_RW_POLL_TIMEOUT_MS;
    uint8_t *tx_buf_ptr;

    if (!client)
        goto _exit;

    start_time = a2e_get_timestamp();

    fds.fd = client->fd;
    fds.events = POLLOUT;

    while(1)
    {
        status = eA2E_SC_ERROR;

        res = poll(&fds, 1, poll_to_ms);
        if (res > 0)
        {
            if (fds.revents & (POLLNVAL | POLLERR))
            {
                a2e_log("%s. Conn write poll failed (POLLNVAL | PLLERR)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (fds.revents & POLLHUP)
            {
                a2e_log("%s. Conn write connection closed by remote side", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            if (!(fds.revents & POLLOUT))
            {
                a2e_log("%s. Conn write output error", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            tx_buf_ptr = tx_buffer + client->req_size_sent;

            sent_len = send(client->fd, tx_buf_ptr, A2E_MIN(A2E_BASE(client)->cfg.rw_chunk_size, size - client->req_size_sent), 0);
            if (sent_len < 0)
            {
                a2e_log("%s. Conn write error (%s)", a2e_name(A2E_BASE(client)));
                goto _exit;
            }

            client->req_size_sent += sent_len;

            a2e_log("%s. Sent data chunk size[%d] (total: %d/%d)", a2e_name(A2E_BASE(client)), sent_len, client->req_size_sent, size);

            if (client->req_size_sent >= size)
            {
                a2e_set_state(A2E_BASE(client), eA2E_STATE_REQ_TX_FINISH);
                status = eA2E_SC_OK;
                goto _exit;
            }
            else
            {
                status = eA2E_SC_CONTINUE;
                /* Cycle will be continued until write end or timeout reached */
            }
        }
        else if (res < 0)
        {
            if (errno != EINTR)
            {
                a2e_log("%s. Conn write poll failed (%s)", a2e_name(A2E_BASE(client)), strerror(errno));
                goto _exit;
            }
        }
        else
        {
            A2E_DBG("%s. Conn write poll timeout (write is not possible)", a2e_name(A2E_BASE(client)));
            /* cycle continue */
        }

        if (a2e_timeout_reached(start_time, to_ms))
        {
            status = (status == eA2E_SC_CONTINUE) ? eA2E_SC_CONTINUE_TIMEOUT: eA2E_SC_TIMEOUT;
            goto _exit;
        }
    }

_exit:
    return status;
}

/** @} */
