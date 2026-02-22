"""
A2E Library - Interface

Abstract base class and strategy interface for A2E instances.
"""

from abc import ABC, abstractmethod
from typing import Optional, Tuple

from .defs import State, Role, A2EConfig, Status
from .dbg import a2e_log


class A2EInstance(ABC):
    """
    Abstract base class for A2E instances.
    
    This class represents the common interface for both client and server
    implementations using the strategy pattern.
    """
    
    def __init__(self):
        self.role: Role = Role.CLIENT
        """Instance role (CLIENT or SERVER)"""
        
        self.state: State = State.NULL
        """Instance state"""
        
        self.cfg: A2EConfig = A2EConfig()
        """Instance configuration"""
    
    @abstractmethod
    def init(self, cfg: A2EConfig) -> Status:
        """
        Initialize the A2E instance.
        
        Args:
            cfg: Configuration for the instance
            
        Returns:
            Status.OK on success, error status otherwise
        """
        pass
    
    @abstractmethod
    def close(self) -> Status:
        """
        Close the A2E instance.
        
        Returns:
            Status.OK on success
        """
        pass
    
    @abstractmethod
    def req_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """
        Receive a request.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Tuple of (status, received data or None)
        """
        pass
    
    @abstractmethod
    def req_tx(self, data: bytes, timeout_ms: int) -> Status:
        """
        Transmit a request.
        
        Args:
            data: Data to send
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        pass
    
    @abstractmethod
    def req_cmplt(self) -> Status:
        """
        Complete the current request.
        
        Returns:
            Status.OK on success
        """
        pass
    
    @abstractmethod
    def req_cmplt_wait(self, timeout_ms: int) -> Status:
        """
        Wait for the peer to complete the request.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        pass
    
    @abstractmethod
    def resp_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """
        Receive a response.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Tuple of (status, received data or None)
        """
        pass
    
    @abstractmethod
    def resp_tx(self, data: bytes, timeout_ms: int) -> Status:
        """
        Transmit a response.
        
        Args:
            data: Data to send
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        pass
    
    @abstractmethod
    def prog_tx(self, timeout_ms: int) -> Status:
        """
        Transmit a progress notification.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        pass


class A2EStrategy(A2EInstance):
    """
    Base class implementing the strategy interface for A2E instances.
    
    Provides common functionality and default implementations.
    """
    
    def __init__(self):
        super().__init__()
    
    def init(self, cfg: A2EConfig) -> Status:
        """Default init - just applies configuration"""
        from .common import cfg_apply, set_state, State
        cfg_apply(self, cfg)
        set_state(self, State.IDLE)
        a2e_log("{}: Initialized with config (sock_dir: {}, name: {})", 
                self.cfg.name, self.cfg.sock_dir, self.cfg.name)
        return Status.OK
    
    def close(self) -> Status:
        """Default close - just sets state to NULL"""
        from .common import set_state, State
        a2e_log("{}: Closing in state {}", self.cfg.name, self.state.name)
        set_state(self, State.NULL)
        return Status.OK
    
    def req_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED, None
    
    def req_tx(self, data: bytes, timeout_ms: int) -> Status:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED
    
    def req_cmplt(self) -> Status:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED
    
    def req_cmplt_wait(self, timeout_ms: int) -> Status:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED
    
    def resp_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED, None
    
    def resp_tx(self, data: bytes, timeout_ms: int) -> Status:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED
    
    def prog_tx(self, timeout_ms: int) -> Status:
        """Not implemented by default"""
        return Status.NOT_IMPLEMENTED
