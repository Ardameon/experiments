"""
A2E Library - Python Implementation

Transport library for providing interaction between processes via UNIX domain sockets.
Supports simple client-server REQUEST-RESPONSE transactions with progress notifications.

Server behavior:
- Listen for connection from client for one REQUEST-RESPONSE transaction
- Can handle only one REQUEST-RESPONSE transaction at a time
- Accept only single connection from one client for single transaction
- Handle request from client
- Can send progress message to client
- Send response to client
- Wait for client disconnect to finish transaction
- Start listening for new connection when transaction is finished

Client behavior:
- Connect to server to start REQUEST-RESPONSE transaction
- Send request to server
- Can receive progress message from server
- Receive response from server
- Disconnect from server to finish transaction
"""

from typing import Optional, Tuple

# Import definitions and types
from .defs import (
    Status,
    State,
    Role,
    MsgType,
    A2EConfig,
    cfg_set_default,
    A2E_SOCK_DIR_LEN_MAX,
    A2E_NAME_LEN_MAX,
    A2E_SOCK_PATH_LEN_MAX,
    A2E_MSG_LEN_MAX,
    A2E_MSG_MAGIC,
    DEF_A2E_SOCK_DIR,
    DEF_A2E_SOCK_NAME,
    DEF_A2E_NAME,
    DEF_A2E_RW_CHUNK_SIZE,
    DEF_A2E_RW_POLL_TIMEOUT_MS,
    DEF_A2E_REQ_CMPLT_WAIT_TRIES,
)

# Import exceptions
from .exceptions import (
    A2EError,
    A2ETimeout,
    A2EContinueTimeout,
    A2EInvalidState,
    A2EInvalidArgs,
    A2ENoMemory,
    A2ENotImplemented,
    A2EIncompleteRead,
    A2EInProgress,
    A2EContinue,
    A2EGenericError,
)

# Import message handling
from .msg import (
    A2EMessage,
    MSG_HEADER_SIZE,
    fill_request,
    fill_response,
    fill_progress,
)

# Import debug functions
from .dbg import (
    dbg_on,
    dbg_off,
    dbg_set_func,
    dbg_is_enabled,
    a2e_log,
    a2e_set_dbg,
    a2e_set_dbg_log_func,
)

# Import client and server
from .client import A2EClient
from .server import A2EServer

# Import base interface
from .iface import A2EInstance, A2EStrategy


# =============================================================================
# Public API Functions
# =============================================================================

def init_client(cfg: Optional[A2EConfig] = None) -> A2EClient:
    """
    Initialize A2E client instance.
    
    Args:
        cfg: Configuration for the client. If None, default config is used.
        
    Returns:
        Initialized A2EClient instance
        
    Raises:
        A2EError: If initialization fails
    """
    client = A2EClient()
    if cfg is None:
        cfg = cfg_set_default()
    
    status = client.init(cfg)
    if status != Status.OK:
        raise A2EError(status, f"Failed to initialize client: {status.name}")
    
    return client


def init_server(cfg: Optional[A2EConfig] = None) -> A2EServer:
    """
    Initialize A2E server instance.
    
    Args:
        cfg: Configuration for the server. If None, default config is used.
        
    Returns:
        Initialized A2EServer instance
        
    Raises:
        A2EError: If initialization fails
    """
    server = A2EServer()
    if cfg is None:
        cfg = cfg_set_default()
    
    status = server.init(cfg)
    if status != Status.OK:
        raise A2EError(status, f"Failed to initialize server: {status.name}")
    
    return server


def close(a2e: A2EInstance) -> Status:
    """
    Close A2E instance.
    
    Args:
        a2e: A2E instance to close
        
    Returns:
        Status.OK on success
    """
    return a2e.close()


def request_tx(a2e: A2EInstance, data: bytes, timeout_ms: int = 500) -> Status:
    """
    Transmit a request.
    
    Args:
        a2e: A2E instance (should be client)
        data: Data to send
        timeout_ms: Timeout in milliseconds
        
    Returns:
        Status.OK on success, error status otherwise
    """
    return a2e.req_tx(data, timeout_ms)


def request_rx(a2e: A2EInstance, timeout_ms: int = 500) -> Tuple[Status, Optional[bytes]]:
    """
    Receive a request.
    
    Args:
        a2e: A2E instance (should be server)
        timeout_ms: Timeout in milliseconds
        
    Returns:
        Tuple of (status, received data or None)
    """
    return a2e.req_rx(timeout_ms)


def request_complete(a2e: A2EInstance) -> Status:
    """
    Complete the current request.
    
    Args:
        a2e: A2E instance
        
    Returns:
        Status.OK on success
    """
    return a2e.req_cmplt()


def request_complete_wait(a2e: A2EInstance, timeout_ms: int = 500) -> Status:
    """
    Wait for the peer to complete the request.
    
    Args:
        a2e: A2E instance (should be server)
        timeout_ms: Timeout in milliseconds
        
    Returns:
        Status.OK on success, error status otherwise
    """
    return a2e.req_cmplt_wait(timeout_ms)


def response_tx(a2e: A2EInstance, data: bytes, timeout_ms: int = 500) -> Status:
    """
    Transmit a response.
    
    Args:
        a2e: A2E instance (should be server)
        data: Data to send
        timeout_ms: Timeout in milliseconds
        
    Returns:
        Status.OK on success, error status otherwise
    """
    return a2e.resp_tx(data, timeout_ms)


def response_rx(a2e: A2EInstance, timeout_ms: int = 500) -> Tuple[Status, Optional[bytes]]:
    """
    Receive a response.
    
    Args:
        a2e: A2E instance (should be client)
        timeout_ms: Timeout in milliseconds
        
    Returns:
        Tuple of (status, received data or None)
    """
    return a2e.resp_rx(timeout_ms)


def progress_tx(a2e: A2EInstance, timeout_ms: int = 500) -> Status:
    """
    Transmit a progress notification.
    
    Args:
        a2e: A2E instance (should be server)
        timeout_ms: Timeout in milliseconds
        
    Returns:
        Status.OK on success, error status otherwise
    """
    return a2e.prog_tx(timeout_ms)


def perror(status: Status) -> str:
    """
    Return string representation of status code.
    
    Args:
        status: Status code
        
    Returns:
        String representation of status
    """
    status_strings = {
        Status.OK: "OK",
        Status.IN_PROGRESS: "IN_PROGRESS",
        Status.TIMEOUT: "TIMEOUT",
        Status.CONTINUE: "CONTINUE",
        Status.CONTINUE_TIMEOUT: "CONTINUE_TIMEOUT",
        Status.NOT_IMPLEMENTED: "NOT_IMPLEMENTED",
        Status.INVALID_ARGS: "INVALID_ARGS",
        Status.NO_MEM: "NO_MEM",
        Status.WRONG_STATE: "WRONG_STATE",
        Status.INCOMPLETE_READ: "INCOMPLETE_READ",
        Status.ERROR: "ERROR",
    }
    return status_strings.get(status, "UNDEF_STATUS")


# =============================================================================
# Module exports
# =============================================================================

__all__ = [
    # Classes
    'A2EClient',
    'A2EServer',
    'A2EConfig',
    'A2EInstance',
    'A2EStrategy',
    'A2EMessage',
    
    # Enums
    'Status',
    'State',
    'Role',
    'MsgType',
    
    # Exceptions
    'A2EError',
    'A2ETimeout',
    'A2EContinueTimeout',
    'A2EInvalidState',
    'A2EInvalidArgs',
    'A2ENoMemory',
    'A2ENotImplemented',
    'A2EIncompleteRead',
    'A2EInProgress',
    'A2EContinue',
    'A2EGenericError',
    
    # Factory functions
    'init_client',
    'init_server',
    
    # API functions
    'close',
    'request_tx',
    'request_rx',
    'request_complete',
    'request_complete_wait',
    'response_tx',
    'response_rx',
    'progress_tx',
    
    # Utility functions
    'perror',
    'cfg_set_default',
    
    # Debug functions
    'dbg_on',
    'dbg_off',
    'dbg_set_func',
    'dbg_is_enabled',
    'a2e_log',
    'a2e_set_dbg',
    'a2e_set_dbg_log_func',
    
    # Message functions
    'fill_request',
    'fill_response',
    'fill_progress',
    
    # Constants
    'MSG_HEADER_SIZE',
    'A2E_SOCK_DIR_LEN_MAX',
    'A2E_NAME_LEN_MAX',
    'A2E_SOCK_PATH_LEN_MAX',
    'A2E_MSG_LEN_MAX',
    'A2E_MSG_MAGIC',
    'DEF_A2E_SOCK_DIR',
    'DEF_A2E_SOCK_NAME',
    'DEF_A2E_NAME',
    'DEF_A2E_RW_CHUNK_SIZE',
    'DEF_A2E_RW_POLL_TIMEOUT_MS',
    'DEF_A2E_REQ_CMPLT_WAIT_TRIES',
]
