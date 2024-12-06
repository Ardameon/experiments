#include <time.h>

#include "a2e_common.h"
#include "a2e_dbg.h"

unsigned long a2e_get_timestamp(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    return ((tp.tv_sec * 1000) + (tp.tv_nsec / 1000000));
}

void a2e_set_state(a2e_t *a2e, a2e_state_e state)
{
    if (a2e->state != state)
    {
        a2e_dbg("%s. State changed: %s => %s", a2e->cfg.name, a2e_state_str(a2e->state), a2e_state_str(state));
        a2e->state = state;
    }
}

const char *a2e_state_str(a2e_state_e state)
{
    switch(state)
    {
        case eA2E_STATE_IDLE:           return "IDLE";
        case eA2E_STATE_REQ_RECV:       return "REQ_RECV";
        case eA2E_STATE_REQ_SENT:       return "REQ_SENT";
        case eA2E_STATE_REQ_PROGRESS:   return "REQ_PROGRESS";
        case eA2E_STATE_REQ_COMPLT:     return "REQ_COMPLT";
        case eA2E_STATE_RESP_RECV:      return "RESP_RECV";
        case eA2E_STATE_RESP_SENT:      return "RESP_SENT";
        default:                        return "UNDEF_STATE";
    }
}
