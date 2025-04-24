/**
 * @file
 *
 * @addtogroup a2e_common
 * @{
 */

#include <time.h>
#include <string.h>

#include "a2e_common.h"
#include "a2e_dbg.h"

/**
 * @copybrief a2e_get_timestamp
 *
 * @return Time from system boot in milliseconds
 */
unsigned long a2e_get_timestamp(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    return ((tp.tv_sec * 1000) + (tp.tv_nsec / 1000000));
}

/**
 * @copybrief a2e_timeout_reached
 *
 * @param[in]  start_timestamp  Timestamp from wich we want to check timeout
 * @param[in]  timeout_ms       Timeout to check
 *
 * @return 0 if timeout is not reached, non-zero otherwise
 */
uint8_t a2e_timeout_reached(unsigned long start_timestamp, unsigned timeout_ms)
{
    return ((a2e_get_timestamp()) - start_timestamp) >= timeout_ms;
}

/**
 * @copybrief a2e_set_state
 *
 * @param[in]  a2e     A2E-instance
 * @param[in]  state   New state
 *
 */
void a2e_set_state(a2e_t *a2e, a2e_state_e state)
{
    if (a2e->state != state)
    {
        a2e_log("%s. State changed: %s => %s", a2e_name(a2e), a2e_state_str(a2e->state), a2e_state_str(state));
        a2e->state = state;
    }
}

/**
 * @copybrief a2e_state_str
 *
 * @param[in]  state  A2E-instance state
 *
 * @return String implementation of state
 */
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
        case eA2E_STATE_REQ_PROGRESS:   return "REQ_PROGRESS";
        case eA2E_STATE_RSP_RX:         return "RSP_RX";
        case eA2E_STATE_RSP_RX_FINISH:  return "RSP_RX_FINISH";
        case eA2E_STATE_RSP_TX:         return "RSP_TX";
        case eA2E_STATE_RSP_TX_FINISH:  return "RSP_TX_FINISH";
        default:                        return "UNDEF_STATE";
    }
}

/**
 * @copybrief a2e_name
 *
 * @param[in]  a2e  A2E-instance
 *
 * @return A2E-instance name
 */
const char *a2e_name(const a2e_t *a2e)
{
    return a2e ? a2e->cfg.name : "nullptr";
}

/**
 * @copybrief a2e_cfg_apply
 *
 * @param[in]  a2e  A2E-instance
 * @param[in]  cfg  A2E-instance configuration to apply
 */
void a2e_cfg_apply(a2e_t *a2e, const a2e_cfg_t *cfg)
{
    memcpy(&a2e->cfg, cfg, sizeof(a2e->cfg));

    if (!a2e->cfg.rw_chunk_size)
    {
        a2e->cfg.rw_chunk_size = DEF_A2E_RW_CHUNK_SIZE;
    }
}

/** @} */
