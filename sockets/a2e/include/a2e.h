/**
 * @file
 *
 * @defgroup A2E A2E library
 *
 * @brief Transport library for providing interraction between @ref access_mediator and ESR OI-manager
 *
 * Library supports simple UNIX stream sockets client and server implementation.
 *
 * Server behavior: @n
 * * Listen for connection from client for one REQUEST-RESPONSE transaction @n
 * * Can handle only one REQEUST-RESPONSE transaction at a time @n
 * * Accept only single connection from one client for single transaction @n
 * * Handle reqeust from client @n
 * * Can send progress message to client @n
 * * Send response to client @n
 * * Wait for client disconnect to finish transaction @n
 * * Start listening for new connection, when transaction (active request) is finished
 *
 * Client behavior: @n
 * * Connect to server to start REQUEST-RESPONSE transaction @n
 * * Send reqeust to server @n
 * * Can receive progress message from server @n
 * * Receive response from server @n
 * * Disconnect from server to finish transaction
 *
 * Transaction(active request session) is: @n
 * * Request (sent from client to server) @n
 * * Progress (optional, sent from server to client to notify about processing) @n
 * * Response (sent from server to client to satisfy reqeust)
 *
 * Functions processing can be interrupted by timeout to handle something in client code and then continue the same functions calls
 *
 * RW-chunks count values in example are demonstrative. RW-chunks count is configurable parameter depends of @ref a2e_cfg_t "rw_chunk_size"
 *
 * @startuml
 * participant "A2E-client" as client #fcf477
 * participant "A2E-server" as server #fcf477
 *
 * Title A2E client-server interaction
 *
 * group Client sends reqeust to server
 * note over server
 * **a2e_request_rx()**
 * end note
 *
 * note over client
 * **a2e_request_tx()**
 * end note
 *
 * client -> server: connect
 *
 * note over client
 * eA2E_SC_CONTINUE
 * end note
 *
 * note over server
 * eA2E_SC_CONTINUE
 * end note
 *
 * note over server
 * **a2e_request_rx()**
 * end note
 *
 * note over client
 * **a2e_request_tx()**
 * end note
 *
 * client -> server: a2e-msg-info (Request)
 *
 * note over client
 * eA2E_SC_CONTINUE
 * end note
 *
 * note over server
 * eA2E_SC_CONTINUE
 * end note
 *
 * note over server
 * **a2e_request_rx()**
 * end note
 *
 * note over client
 * **a2e_request_tx()**
 * end note
 *
 * client -> server: Request (chunk 1/8)
 * client -> server: Request (chunk 2/8)
 * client -> server: Request (chunk 3/8)
 * client -> server: Request (chunk 4/8)
 * client -> server: Request (chunk 5/8)
 * client -> server: Request (chunk 6/8)
 * client -> server: Request (chunk 7/8)
 * client -> server: Request (chunk 8/8)
 *
 * note over client
 * eA2E_SC_OK
 * end note
 *
 * note over server
 * eA2E_SC_OK
 * end note
 * end
 *
 * group Client receives response to request
 * note over client
 * **a2e_response_rx()**
 * end note
 *
 * group Server sends Progress response (optional)
 * note over server
 * **a2e_progress_tx()**
 * end note
 *
 * client <- server: a2e-msg-info (Progress)
 *
 * note over server
 * eA2E_SC_OK
 * end note
 *
 * note over client
 * eA2E_SC_IN_PROGRESS
 * end note
 * end
 *
 * note over client
 * **a2e_response_rx()**
 * end note
 *
 * note over server
 * **a2e_response_tx()**
 * end note
 *
 * client <- server: a2e-msg-info (Response)
 *
 * note over server
 * eA2E_SC_CONTINUE
 * end note
 *
 * note over client
 * eA2E_SC_CONTINUE
 * end note
 *
 * note over client
 * **a2e_response_rx()**
 * end note
 *
 * note over server
 * **a2e_response_tx()**
 * end note
 *
 * client <- server: Response (chunk 1/4)
 * client <- server: Response (chunk 2/4)
 * client <- server: Response (chunk 3/4)
 * client <- server: Response (chunk 4/4)
 *
 * note over server
 * eA2E_SC_OK
 * end note
 *
 * note over client
 * eA2E_SC_OK
 * end note
 * end
 *
 * group Server waits for request complete from client
 * note over server
 * **a2e_request_complete_wait()**
 * end note
 *
 * note over client
 * **a2e_reqeust_complete()**
 * end note
 *
 * client -> server: disconnect
 *
 * note over client
 * eA2E_SC_OK
 * end note
 *
 * note over server
 * eA2E_SC_OK
 * end note
 * end
 *
 * @enduml
 *
 * @defgroup a2e_pub A2E library interface
 *
 * @brief Library public functions that can be used in client code
 *
 * @ingroup A2E
 * @{
 */
#ifndef __A2E_H__
#define __A2E_H__

#include <stdint.h>

#include "a2e_def.h"

/** @brief Interface function for A2E UNIX stream socket client initialization */
a2e_status_e a2e_init_client(a2e_t **a2e, const a2e_cfg_t *cfg);
/** @brief Interface function for A2E UNIX stream socket server initialization */
a2e_status_e a2e_init_server(a2e_t **a2e, const a2e_cfg_t *cfg);

/** @brief Interface function for closing A2E-instance */
a2e_status_e a2e_close(a2e_t *a2e);

/** @brief Interface function for receiving requests for A2E-instance from it's peer */
a2e_status_e a2e_request_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
/** @brief Interface function for sending requests from A2E-instance to it's peer */
a2e_status_e a2e_request_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);
/** @brief Interface function for completing current request porcessing on A2E-instance */
a2e_status_e a2e_request_complete(a2e_t *a2e);
/** @brief Interface function for waiting current request complete from peer side of A2E-instance */
a2e_status_e a2e_request_complete_wait(a2e_t *a2e, uint16_t to_ms);

/** @brief Interface function for receiving active request responses for A2E-instanse from it's peer */
a2e_status_e a2e_response_rx(a2e_t *a2e, uint8_t **rx_buffer, uint32_t *size, uint16_t to_ms);
/** @brief Interface function for sending active request responses from A2E-instance to it's peer */
a2e_status_e a2e_response_tx(a2e_t *a2e, uint8_t *tx_buffer, uint32_t size, uint16_t to_ms);

/** @brief Interface function for sending active request porgress response from A2E-instance to it's peer */
a2e_status_e a2e_progress_tx(a2e_t *a2e, uint16_t to_ms);

/** @brief Fill A2E-instance configuration with default values */
void a2e_cfg_set_default(a2e_cfg_t *cfg);

/** @brief Turn A2E library debug logs on */
void a2e_dbg_on(void);
/** @brief Turn A2E library debug logs off*/
void a2e_dbg_off(void);
/** @brief Set log function for debug logs of A2E library */
void a2e_dbg_set_func(a2e_log_func log_func);

/** @brief Return string implementation of @ref a2e_status_e "status" */
const char *a2e_perror(a2e_status_e status);


#endif /* __A2E_H__ */

/** @} */
