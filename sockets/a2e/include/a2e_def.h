/**
 * @file
 *
 * @defgroup a2e_defs A2E library types
 *
 * @brief Library public data structures and types
 *
 * @ingroup A2E
 * @{
 */

#ifndef __A2E_DEF_H__
#define __A2E_DEF_H__

/** @brief Maximum UNIX socket path length */
#define A2E_SOCK_DIR_LEN_MAX 64
/** @brief Maximum @ref a2e_t "A2E-instance" name length */
#define A2E_NAME_LEN_MAX 32

/** @brief Library logging function type */
typedef void (*a2e_log_func)(const char *log_str, int len);

/** @brief Library status code */
typedef enum
{
    /** Success */
    eA2E_SC_OK,
    /** Progress response received */
    eA2E_SC_IN_PROGRESS,
    /** Function call timeout acquired  */
    eA2E_SC_TIMEOUT,
    /** Action successfully started, call function more to continue action handling */
    eA2E_SC_CONTINUE,
    /** Function call timeout acquired during action handling, call function more to continue action handling */
    eA2E_SC_CONTINUE_TIMEOUT,
    /** Function is not implemented for specific @ref a2e_t "A2E-instance" */
    eA2E_SC_NOT_IMPLEMENTED,
    /** Arguments of function are invalid */
    eA2E_SC_INVALID_ARGS,
    /** Failed to allocate memory */
    eA2E_SC_NO_MEM,
    /** Function is not provided for current @ref a2e_t "A2E-instance" @ref a2e_state_e "state"*/
    eA2E_SC_WRONG_STATE,
    /** There is still data for receive from active file descriptor */
    eA2E_SC_INCOMPLETE_READ,
    /** General error */
    eA2E_SC_ERROR,
} a2e_status_e;

/** @brief @ref a2e_t "A2E-instance" role */
typedef enum
{
    /** Server */
    eA2E_ROLE_SERVER,
    /** Client */
    eA2E_ROLE_CLIENT,
} a2e_role_e;

/** @brief @ref a2e_t "A2E-instance" state */
typedef enum
{
    /** Instance was created but not initialized */
    eA2E_STATE_NULL,
    /** Instance is initialized and ready for actions */
    eA2E_STATE_IDLE,
    /** Request receiving procedure started (connection established) */
    eA2E_STATE_REQ_RX_START,
    /** Request receiving procedure in progress */
    eA2E_STATE_REQ_RX,
    /** Request receiving procedure finished, request is fully obtained */
    eA2E_STATE_REQ_RX_FINISH,
    /** Request transmitting procedure started (connection established) */
    eA2E_STATE_REQ_TX_START,
    /** Request transmitting procedure in progress */
    eA2E_STATE_REQ_TX,
    /** Request transmitting procedure finished, request is fully sent */
    eA2E_STATE_REQ_TX_FINISH,
    /** Request progress response sent/received */
    eA2E_STATE_REQ_PROGRESS,
    /** Response receiving procedure in progress */
    eA2E_STATE_RSP_RX,
    /** Response receiving procedure finished, response is fully obtained */
    eA2E_STATE_RSP_RX_FINISH,
    /** Response transmitting procedure in progress */
    eA2E_STATE_RSP_TX,
    /** Response transmitting procedure finished, response is fully sent */
    eA2E_STATE_RSP_TX_FINISH,
} a2e_state_e;

/** @brief @ref a2e_t "A2E-instance" configuration */
typedef struct a2e_cfg_t
{
    /** Instance name (used in logs) */
    char name[A2E_NAME_LEN_MAX];
    /** UNIX socket path */
    char sock_dir[A2E_SOCK_DIR_LEN_MAX];

    /** RX/TX chunk size for one send/recv call */
    unsigned rw_chunk_size;
    /** How many times server can acquire timeout on @ref a2e_request_complete_wait "waiting request complete from client"
     *  before forcely complete request by itself
     */
    unsigned req_cmplt_wait_tries;
} a2e_cfg_t;

/** @brief A2E-instance data structure */
typedef struct a2e_t
{
    /** Instance @ref a2e_role_e "role" */
    a2e_role_e  role;
    /** Instance @ref a2e_state_e "state" */
    a2e_state_e state;
    /** Instance @ref a2e_cfg_t "configuration" */
    a2e_cfg_t   cfg;
} a2e_t;

#endif /* __A2E_DEF_H__ */

/** @} */
