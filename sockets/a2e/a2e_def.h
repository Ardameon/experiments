#ifndef __A2E_DEF_H__
#define __A2E_DEF_H__

typedef void (*a2e_log_func)(const char *log_str, int len);

typedef enum
{
    eA2E_STATUS_OK,
    eA2E_STATUS_NOT_IMPLEMENTED,
    eA2E_STATUS_INVALID_ARGS,
    eA2E_STATUS_ERROR,
} a2e_status_e;

typedef enum
{
    eA2E_ROLE_SERVER,
    eA2E_ROLE_CLIENT,
} a2e_role_e;

typedef enum
{
    eA2E_STATE_IDLE,
} a2e_state_e;

typedef struct a2e_t
{
    a2e_role_e  role;
    a2e_state_e state;
} a2e_t;


#endif /* __A2E_DEF_H__ */
