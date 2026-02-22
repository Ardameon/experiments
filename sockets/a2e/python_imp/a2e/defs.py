"""
A2E Library - Definitions

Constants, enums and type definitions for A2E library.
"""

from enum import Enum, IntEnum
from dataclasses import dataclass, field


# =============================================================================
# Constants
# =============================================================================

A2E_SOCK_DIR_LEN_MAX = 64
"""Maximum UNIX socket path length"""

A2E_NAME_LEN_MAX = 32
"""Maximum A2E-instance name length"""

A2E_SOCK_PATH_LEN_MAX = 96
"""System maximum socket path length"""

A2E_MSG_LEN_MAX = (1 << 32) - 1
"""Message max length (UINT_MAX)"""

A2E_MSG_MAGIC = 0xBADA77BC
"""Internal message magic for validation"""

DEF_A2E_SOCK_DIR = "/tmp"
"""Default UNIX socket path"""

DEF_A2E_SOCK_NAME = "a2e.sock"
"""Default UNIX socket name"""

DEF_A2E_NAME = "A2E"
"""Default A2E-instance name"""

DEF_A2E_RW_CHUNK_SIZE = 64 * 1024
"""Default RX/TX data chunk size (64 KB)"""

DEF_A2E_RW_POLL_TIMEOUT_MS = 50
"""Default socket poll timeout (ms)"""

DEF_A2E_REQ_CMPLT_WAIT_TRIES = 3
"""Default request complete wait tries count for server"""


# =============================================================================
# Status Codes
# =============================================================================

class Status(IntEnum):
    """Library status code"""
    OK = 0
    """Success"""
    
    IN_PROGRESS = 1
    """Progress response received"""
    
    TIMEOUT = 2
    """Function call timeout acquired"""
    
    CONTINUE = 3
    """Action successfully started, call function more to continue action handling"""
    
    CONTINUE_TIMEOUT = 4
    """Function call timeout acquired during action handling, call function more"""
    
    NOT_IMPLEMENTED = 5
    """Function is not implemented for specific A2E-instance"""
    
    INVALID_ARGS = 6
    """Arguments of function are invalid"""
    
    NO_MEM = 7
    """Failed to allocate memory"""
    
    WRONG_STATE = 8
    """Function is not provided for current A2E-instance state"""
    
    INCOMPLETE_READ = 9
    """There is still data for receive from active file descriptor"""
    
    ERROR = 10
    """General error"""


# =============================================================================
# Role
# =============================================================================

class Role(IntEnum):
    """A2E-instance role"""
    SERVER = 0
    """Server"""
    
    CLIENT = 1
    """Client"""


# =============================================================================
# State
# =============================================================================

class State(IntEnum):
    """A2E-instance state"""
    NULL = 0
    """Instance was created but not initialized"""
    
    IDLE = 1
    """Instance is initialized and ready for actions"""
    
    REQ_RX_START = 2
    """Request receiving procedure started (connection established)"""
    
    REQ_RX = 3
    """Request receiving procedure in progress"""
    
    REQ_RX_FINISH = 4
    """Request receiving procedure finished, request is fully obtained"""
    
    REQ_TX_START = 5
    """Request transmitting procedure started (connection established)"""
    
    REQ_TX = 6
    """Request transmitting procedure in progress"""
    
    REQ_TX_FINISH = 7
    """Request transmitting procedure finished, request is fully sent"""
    
    REQ_PROGRESS = 8
    """Request progress response sent/received"""
    
    RSP_RX = 9
    """Response receiving procedure in progress"""
    
    RSP_RX_FINISH = 10
    """Response receiving procedure finished, response is fully obtained"""
    
    RSP_TX = 11
    """Response transmitting procedure in progress"""
    
    RSP_TX_FINISH = 12
    """Response transmitting procedure finished, response is fully sent"""


# =============================================================================
# Message Type
# =============================================================================

class MsgType(IntEnum):
    """Message type"""
    REQUEST = 0
    """Request. Should only be sent by client."""
    
    RESPONSE = 1
    """Response. Should only be sent by server in answer to received request."""
    
    PROGRESS = 2
    """Progress. Should only be sent by server in answer to received request."""


# =============================================================================
# Configuration
# =============================================================================

@dataclass
class A2EConfig:
    """A2E-instance configuration"""
    
    name: str = field(default_factory=lambda: DEF_A2E_NAME)
    """Instance name (used in logs)"""
    
    sock_dir: str = field(default_factory=lambda: DEF_A2E_SOCK_DIR)
    """UNIX socket path"""
    
    rw_chunk_size: int = DEF_A2E_RW_CHUNK_SIZE
    """RX/TX chunk size for one send/recv call"""
    
    req_cmplt_wait_tries: int = DEF_A2E_REQ_CMPLT_WAIT_TRIES
    """How many times server can acquire timeout on waiting request complete"""
    
    def __post_init__(self):
        if len(self.name) > A2E_NAME_LEN_MAX:
            self.name = self.name[:A2E_NAME_LEN_MAX]
        if len(self.sock_dir) > A2E_SOCK_DIR_LEN_MAX:
            self.sock_dir = self.sock_dir[:A2E_SOCK_DIR_LEN_MAX]


def cfg_set_default() -> A2EConfig:
    """Fill A2E-instance configuration with default values"""
    return A2EConfig()
