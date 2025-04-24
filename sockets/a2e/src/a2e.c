/**
 * @file
 *
 * @brief The main source file of A2E library
 *
 * @addtogroup a2e_pub
 * @{
 */

// ---------------------includes------------------------

#include <string.h>

#include "a2e.h"
#include "a2e_iface.h"
#include "a2e_client.h"
#include "a2e_server.h"
#include "a2e_dbg.h"
#include "a2e_common.h"

// ---------------------defines-------------------------

// ---------------------types---------------------------

/** @brief Extension of base @ref a2e_t "A2E-instance" structure to provide access to @ref a2e_strategy_i "interface" */
typedef struct a2e_ext_t
{
    /** Base structure */
    a2e_t base;
    /** Extension (interface) */
    a2e_strategy_i iface;
} a2e_ext_t;

// ---------------------prototypes----------------------
// ---------------------global vars---------------------
// ---------------------functions-----------------------

/**
 * @copybrief a2e_init_client
 *
 * @param[out]  a2e  Pointer to newly allocated and initialized client
 * @param[in]   cfg  Configuration for new client
 *
 * @return @ref eA2E_SC_OK on success
 */
a2e_status_e a2e_init_client(a2e_t **a2e, const a2e_cfg_t *cfg)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_strategy_i client_iface = a2e_client_iface_get();

    if (client_iface.init)
    {
        status = (a2e_status_e)client_iface.init(a2e, cfg);
    }

    return status;
}

/**
 * @copybrief a2e_init_server
 *
 * @param[out]  a2e  Pointer to newly allocated and initialized server
 * @param[in]   cfg  Configuration for new client
 *
 * @return @ref eA2E_SC_OK on success
 */
a2e_status_e a2e_init_server(a2e_t **a2e, const a2e_cfg_t *cfg)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_strategy_i server_iface = a2e_server_iface_get();

    if (server_iface.init)
    {
        status = (a2e_status_e)server_iface.init(a2e, cfg);
    }

    return status;
}

/**
 * @copybrief a2e_request_rx
 *
 * @param[in]   a2e        A2E-instanse
 * @param[out]  rx_buffer  Pointer to buffer with received request
 * @param[out]  size       Received buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * @return @ref eA2E_SC_OK on successful request receiving, after that we can handle rx_buffer
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue receiving session)
 *        @n @ref eA2E_SC_CONTINUE on successful peer connecting and receiving information about upcoming request
 *        @n @ref eA2E_SC_CONTINUE_TIMEOUT on function timeout acquired during active receiving session
 *        @n Other status codes should be interpreted as errors
 */
a2e_status_e a2e_request_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e || !rx_buffer || !size)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_rx)
    {
        status = (a2e_status_e)ext->iface.req_rx(a2e, rx_buffer, size, to_ms);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_request_tx
 *
 * @param[in]   a2e        A2E-instanse
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
a2e_status_e a2e_request_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e || !tx_buffer)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_tx)
    {
        status = (a2e_status_e)ext->iface.req_tx(a2e, tx_buffer, size, to_ms);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_request_complete
 *
 * @param[in]   a2e        A2E-instanse
 *
 * @return @ref eA2E_SC_OK on success
 */
a2e_status_e a2e_request_complete(a2e_t *a2e)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_cmplt)
    {
        status = (a2e_status_e)ext->iface.req_cmplt(a2e);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_request_complete_wait
 *
 * @param[in]   a2e        A2E-instanse
 * @param[in]   to_ms      Timeout of one wait try in milliseconds
 *
 * @return @ref eA2E_SC_OK on successful active request complete received from peer
 *        @n @ref eA2E_SC_INCOMPLETE_READ there is still data for read in file descriptor (should interpret as error, client is trying to send more then we expect)
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired during waiting of request complete from peer
 *        @n Other status codes should be interpreted as errors
 */
a2e_status_e a2e_request_complete_wait(a2e_t *a2e, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.req_cmplt_wait)
    {
        status = (a2e_status_e)ext->iface.req_cmplt_wait(a2e, to_ms);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_response_rx
 *
 * @param[in]   a2e        A2E-instanse
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
a2e_status_e a2e_response_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e || !rx_buffer || !size)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.resp_rx)
    {
        status = (a2e_status_e)ext->iface.resp_rx(a2e, rx_buffer, size, to_ms);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_response_tx
 *
 * @param[in]   a2e        A2E-instanse
 * @param[in]   tx_buffer  Pointer to buffer with response to send
 * @param[in]   size       Buffer size
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * @return @ref eA2E_SC_OK on successful response sending
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue sending session)
 *        @n @ref eA2E_SC_CONTINUE on successful connecting to peer and sending information about upcoming response
 *        @n @ref eA2E_SC_CONTINUE_TIMEOUT on function timeout acquired during active sending session
 *        @n Other status codes should be interpreted as errors
 */
a2e_status_e a2e_response_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e || !tx_buffer)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.resp_tx)
    {
        status = (a2e_status_e)ext->iface.resp_tx(a2e, tx_buffer, size, to_ms);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_progress_tx
 *
 * @param[in]   a2e        A2E-instanse
 * @param[in]   to_ms      Timeout for function call in milliseconds
 *
 * @return @ref eA2E_SC_OK on successful progress sending
 *        @n @ref eA2E_SC_TIMEOUT on function timeout acquired (can't start or continue sending session)
 *        @n Other status codes should be interpreted as errors
 */
a2e_status_e a2e_progress_tx(a2e_t *a2e, uint16_t to_ms)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.prog_tx)
    {
        status = (a2e_status_e)ext->iface.prog_tx(a2e, to_ms);
    }

_exit:
    return status;
}

/**
 * @copybrief a2e_close
 *
 * @param[in]   a2e        A2E-instanse to close
 *
 * @return @ref eA2E_SC_OK on success
 */
a2e_status_e a2e_close(a2e_t *a2e)
{
    a2e_status_e status = eA2E_SC_NOT_IMPLEMENTED;
    a2e_ext_t *ext = (a2e_ext_t *)a2e;

    if (!a2e)
    {
        status = eA2E_SC_INVALID_ARGS;
        goto _exit;
    }

    if (ext->iface.close)
    {
        status = (a2e_status_e)ext->iface.close(a2e);
    }

_exit:
    return status;

}

/**
 * @copybrief a2e_dbg_on
 */
void a2e_dbg_on(void)
{
    a2e_set_dbg(1);
}

/**
 * @copybrief a2e_dbg_off
 */
void a2e_dbg_off(void)
{
    a2e_set_dbg(0);
}

/**
 * @copybrief a2e_dbg_set_func
 */
void a2e_dbg_set_func(a2e_log_func log_func)
{
    a2e_set_dbg_log_func(log_func);
}

/**
 * @copybrief a2e_perror
 *
 * @param[in]   status        @ref a2e_status_e "Status" code to implement as string
 *
 * @return String implementations of status
 */
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

/**
 * @copybrief a2e_cfg_set_default
 *
 * Dfault values:
 * @n @ref DEF_A2E_SOCK_DIR
 * @n @ref DEF_A2E_NAME
 * @n @ref DEF_A2E_RW_CHUNK_SIZE
 * @n @ref DEF_A2E_REQ_CMPLT_WAIT_TRIES
 *
 * @param[in]   cfg        Configuration for A2E-instance creation
 */
void a2e_cfg_set_default(a2e_cfg_t *cfg)
{
    if (cfg)
    {
        strncpy(cfg->sock_dir, DEF_A2E_SOCK_DIR, sizeof(cfg->sock_dir));
        strncpy(cfg->name, DEF_A2E_NAME, sizeof(cfg->name));
        cfg->rw_chunk_size = DEF_A2E_RW_CHUNK_SIZE;
        cfg->req_cmplt_wait_tries = DEF_A2E_REQ_CMPLT_WAIT_TRIES;
    }
}

/** @} */
