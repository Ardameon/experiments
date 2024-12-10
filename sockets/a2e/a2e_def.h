#ifndef __A2E_DEF_H__
#define __A2E_DEF_H__

#define A2E_SOCK_DIR_LEN_MAX 64
#define A2E_NAME_LEN_MAX 32

typedef void (*a2e_log_func)(const char *log_str, int len);

typedef enum
{
    eA2E_SC_OK,
    eA2E_SC_IN_PROGRESS,
    eA2E_SC_TIMEOUT,
    eA2E_SC_NOT_IMPLEMENTED,
    eA2E_SC_INVALID_ARGS,
    eA2E_SC_NO_MEM,
    eA2E_SC_WRONG_STATE,
    eA2E_SC_INCOMPLETE_READ,
    eA2E_SC_CONTINUE,
    eA2E_SC_CONTINUE_TIMEOUT,
    eA2E_SC_ERROR,
} a2e_status_e;

typedef enum
{
    eA2E_ROLE_SERVER,
    eA2E_ROLE_CLIENT,
} a2e_role_e;

typedef enum
{
    eA2E_STATE_NULL,
    eA2E_STATE_IDLE,
    eA2E_STATE_REQ_RX_START,
    eA2E_STATE_REQ_RX,
    eA2E_STATE_REQ_RX_FINISH,
    eA2E_STATE_REQ_TX_START,
    eA2E_STATE_REQ_TX,
    eA2E_STATE_REQ_TX_FINISH,
    eA2E_STATE_REQ_SENT,
    eA2E_STATE_REQ_PROGRESS,
    eA2E_STATE_REQ_COMPLT,
    eA2E_STATE_RSP_RX,
    eA2E_STATE_RSP_RX_FINISH,
    eA2E_STATE_RSP_TX,
    eA2E_STATE_RSP_TX_FINISH,
} a2e_state_e;

typedef struct a2e_cfg_t
{
    char name[A2E_NAME_LEN_MAX + 1];
    char sock_dir[A2E_SOCK_DIR_LEN_MAX + 1];

    unsigned rw_chunk_size;
} a2e_cfg_t;

typedef struct a2e_t
{
    a2e_role_e  role;
    a2e_state_e state;
    a2e_cfg_t   cfg;
} a2e_t;



#endif /* __A2E_DEF_H__ */
