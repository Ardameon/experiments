/**
 * @file
 *
 * @defgroup a2e_msg Message
 *
 * @brief Library message type and logic
 *
 * @ingroup a2e_internal
 * @{
 */
#ifndef __A2E_MESSAGE_H__
#define __A2E_MESSAGE_H__

#include <stdint.h>
#include <limits.h>

/** @brief Message max length */
#define A2E_MSG_LEN_MAX UINT_MAX
/** @brief Internal message magic */
#define A2E_MSG_MAGIC 0xBADA77BC

/** @brief Message type */
typedef enum
{
    /** Request. Should only be sent by client. */
    eA2E_MSG_REQEUST,
    /** Response. Should only be sent by server in answer to received request. */
    eA2E_MSG_RESPONSE,
    /** Progress. Should only be sent by server in answer to received request to notify client about it's processing */
    eA2E_MSG_PROGRESS,
} a2e_msg_type_e;

/** @brief A2E message structure
 *
 * Basicly this structure is a header of real message that will be send or received by A2E-instances. @n
 * To transfer message instance will send the header first and then send opaque message body of length @n
 * specified in message header.
 *
 * */
typedef struct a2e_msg_t
{
    /** Type to define the message */
    a2e_msg_type_e type;
    /** Magic for validation */
    uint32_t magic;
    /** Message length */
    uint32_t len;
}
/** @cond */
__attribute__((__packed__))
/** @endcond */
a2e_msg_t;

/** @brief Fill request message with data */
void a2e_msg_fill_req(a2e_msg_t *msg, uint32_t len);
/** @brief Fill response message with data */
void a2e_msg_fill_rsp(a2e_msg_t *msg, uint32_t len);
/** @brief Fill progress message with data */
void a2e_msg_fill_prg(a2e_msg_t *msg);


#endif /* __A2E_MESSAGE_H__ */

/** @} */
