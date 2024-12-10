#include <time.h>
#include <string.h>

#include "a2e_common.h"
#include "a2e_dbg.h"

unsigned long a2e_get_timestamp(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    return ((tp.tv_sec * 1000) + (tp.tv_nsec / 1000000));
}

uint8_t a2e_timeout_reached(unsigned long start_timestamp, unsigned timeout_ms)
{
    return ((a2e_get_timestamp()) - start_timestamp) >= timeout_ms;
}

void a2e_set_state(a2e_t *a2e, a2e_state_e state)
{
    if (a2e->state != state)
    {
        a2e_dbg("%s. State changed: %s => %s", a2e_name(a2e), a2e_state_str(a2e->state), a2e_state_str(state));
        a2e->state = state;
    }
}

const char *a2e_state_str(a2e_state_e state)
{
    switch(state)
    {
        case eA2E_STATE_NULL:           return "NULL";
        case eA2E_STATE_IDLE:           return "IDLE";
        case eA2E_STATE_REQ_RX_START:   return "REQ_RX_START";
        case eA2E_STATE_REQ_RX:         return "REQ_RX";
        case eA2E_STATE_REQ_RX_FINISH:  return "REQ_RX_FINISH";
        case eA2E_STATE_REQ_TX_START:   return "REQ_TX_START";
        case eA2E_STATE_REQ_TX:         return "REQ_TX";
        case eA2E_STATE_REQ_TX_FINISH:  return "REQ_TX_FINISH";
        case eA2E_STATE_REQ_SENT:       return "REQ_SENT";
        case eA2E_STATE_REQ_PROGRESS:   return "REQ_PROGRESS";
        case eA2E_STATE_REQ_COMPLT:     return "REQ_COMPLT";
        case eA2E_STATE_RSP_RX:         return "RSP_RX";
        case eA2E_STATE_RSP_RX_FINISH:  return "RSP_RX_FINISH";
        case eA2E_STATE_RSP_TX:         return "RSP_TX";
        case eA2E_STATE_RSP_TX_FINISH:  return "RSP_TX_FINISH";
        default:                        return "UNDEF_STATE";
    }
}

const char *a2e_name(const a2e_t *a2e)
{
    return a2e ? a2e->cfg.name : "nullptr";
}

void a2e_cfg_apply(a2e_t *a2e, const a2e_cfg_t *cfg)
{
    memcpy(&a2e->cfg, cfg, sizeof(*cfg));

    if (!a2e->cfg.rw_chunk_size)
    {
        a2e->cfg.rw_chunk_size = DEF_A2E_RW_CHUNK_SIZE;
    }
}
