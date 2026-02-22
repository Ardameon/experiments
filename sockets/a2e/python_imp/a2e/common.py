"""
A2E Library - Common

Common functions and utilities used throughout the A2E library.
"""

import time
from typing import TYPE_CHECKING

from .defs import State, A2EConfig, DEF_A2E_RW_CHUNK_SIZE
from .dbg import a2e_log

# Avoid circular imports
if TYPE_CHECKING:
    from .iface import A2EInstance


def get_timestamp() -> int:
    """
    Get current time from system start in milliseconds.
    
    Returns:
        Time from system boot in milliseconds
    """
    return int(time.monotonic() * 1000)


def timeout_reached(start_timestamp: int, timeout_ms: int) -> bool:
    """
    Check if timeout since start timestamp is reached.
    
    Args:
        start_timestamp: Timestamp from which we want to check timeout
        timeout_ms: Timeout to check in milliseconds
        
    Returns:
        True if timeout is reached, False otherwise
    """
    return (get_timestamp() - start_timestamp) >= timeout_ms


def set_state(a2e: 'A2EInstance', state: State) -> None:
    """
    Set A2E-instance state.

    Args:
        a2e: A2E-instance
        state: New state
    """
    if a2e.state != state:
        a2e_log("{}: State changed: {} => {}", name(a2e), state_str(a2e.state), state_str(state))
        a2e.state = state


def state_str(state: State) -> str:
    """
    Get string implementation of A2E-instance state.
    
    Args:
        state: A2E-instance state
        
    Returns:
        String representation of state
    """
    state_names = {
        State.NULL: "NULL",
        State.IDLE: "IDLE",
        State.REQ_RX_START: "REQ_RX_START",
        State.REQ_RX: "REQ_RX",
        State.REQ_RX_FINISH: "REQ_RX_FINISH",
        State.REQ_TX_START: "REQ_TX_START",
        State.REQ_TX: "REQ_TX",
        State.REQ_TX_FINISH: "REQ_TX_FINISH",
        State.REQ_PROGRESS: "REQ_PROGRESS",
        State.RSP_RX: "RSP_RX",
        State.RSP_RX_FINISH: "RSP_RX_FINISH",
        State.RSP_TX: "RSP_TX",
        State.RSP_TX_FINISH: "RSP_TX_FINISH",
    }
    return state_names.get(state, "UNDEF_STATE")


def name(a2e: 'A2EInstance') -> str:
    """
    Get A2E-instance name.
    
    Args:
        a2e: A2E-instance
        
    Returns:
        A2E-instance name or 'nullptr' if a2e is None
    """
    if a2e is None:
        return "nullptr"
    return a2e.cfg.name


def cfg_apply(a2e: 'A2EInstance', cfg: A2EConfig) -> None:
    """
    Apply config for A2E-instance.
    
    Args:
        a2e: A2E-instance
        cfg: A2E-instance configuration to apply
    """
    a2e.cfg.name = cfg.name
    a2e.cfg.sock_dir = cfg.sock_dir
    a2e.cfg.rw_chunk_size = cfg.rw_chunk_size if cfg.rw_chunk_size > 0 else DEF_A2E_RW_CHUNK_SIZE
    a2e.cfg.req_cmplt_wait_tries = cfg.req_cmplt_wait_tries
