"""
A2E Library - Client

UNIX domain socket client implementation for A2E library.
"""

import socket
import select
import os
from typing import Optional, Tuple

from .defs import State, Role, Status, A2EConfig, MsgType
from .iface import A2EInstance
from .msg import A2EMessage, MSG_HEADER_SIZE
from .common import get_timestamp, timeout_reached, set_state, a2e_log, name, cfg_apply
from .dbg import a2e_log as log
from .exceptions import A2EInvalidState, A2EInvalidArgs


class A2EClient(A2EInstance):
    """
    A2E client implementation.
    
    Connects to a server via UNIX domain socket and performs
    REQUEST-RESPONSE transactions.
    """
    
    def __init__(self):
        super().__init__()
        self.role = Role.CLIENT
        self.state = State.NULL
        
        self.sock_path: str = ""
        """Instance socket path"""
        
        self.fd: Optional[socket.socket] = None
        """Socket file descriptor"""
        
        self.rsp: Optional[bytes] = None
        """Received response buffer"""
        
        self.rsp_size_recv: int = 0
        """Size of received response"""
        
        self.rsp_size_exp: int = 0
        """Expected size of response"""
        
        self.req_size_sent: int = 0
        """Size of sent request"""
        
        self._req_data: Optional[bytes] = None
        """Request data being sent"""
    
    def init(self, cfg: A2EConfig) -> Status:
        """
        Initialize the A2E client.
        
        Args:
            cfg: Configuration for the client
            
        Returns:
            Status.OK on success, error status otherwise
        """
        if cfg is None:
            return Status.INVALID_ARGS
        
        cfg_apply(self, cfg)
        self.role = Role.CLIENT
        self.state = State.NULL
        
        status = self._start()
        if status != Status.OK:
            return status
        
        set_state(self, State.IDLE)
        log("{}: Client initialized (unix: '{}')", name(self), self.cfg.sock_dir)
        return Status.OK
    
    def close(self) -> Status:
        """
        Close the A2E client.
        
        Returns:
            Status.OK on success
        """
        self._stop()
        return Status.OK
    
    def req_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """
        Receive a request (not implemented for client).
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.NOT_IMPLEMENTED, None
        """
        return Status.NOT_IMPLEMENTED, None
    
    def req_tx(self, data: bytes, timeout_ms: int) -> Status:
        """
        Transmit a request to the server.
        
        Args:
            data: Data to send
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        if data is None:
            return Status.INVALID_ARGS
        
        if self.state == State.IDLE:
            status = self._connect()
            if status != Status.CONTINUE:
                return status
        
        if self.state == State.REQ_TX_START:
            status = self._write_start(data, timeout_ms)
            return status
        
        if self.state == State.REQ_TX:
            status = self._write(data, timeout_ms)
            return status
        
        return Status.WRONG_STATE
    
    def req_cmplt(self) -> Status:
        """
        Complete the current request.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.state in (State.RSP_RX, State.RSP_RX_FINISH, State.REQ_PROGRESS,
                          State.REQ_TX, State.REQ_TX_FINISH):
            self._cleanup()
            set_state(self, State.IDLE)
            return Status.OK
        
        return Status.WRONG_STATE
    
    def req_cmplt_wait(self, timeout_ms: int) -> Status:
        """
        Wait for the server to complete the request (not implemented for client).
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.NOT_IMPLEMENTED
        """
        return Status.NOT_IMPLEMENTED
    
    def resp_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """
        Receive a response from the server.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Tuple of (status, received data or None)
        """
        if self.state in (State.REQ_TX_FINISH, State.REQ_PROGRESS):
            status = self._read_start(timeout_ms)
            return status, None
        
        if self.state == State.RSP_RX:
            status = self._read(timeout_ms)
            if status == Status.OK:
                return status, self.rsp
            return status, None
        
        return Status.WRONG_STATE, None
    
    def resp_tx(self, data: bytes, timeout_ms: int) -> Status:
        """
        Transmit a response (not implemented for client).
        
        Args:
            data: Data to send
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.NOT_IMPLEMENTED
        """
        return Status.NOT_IMPLEMENTED
    
    def prog_tx(self, timeout_ms: int) -> Status:
        """
        Transmit a progress notification (not implemented for client).
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.NOT_IMPLEMENTED
        """
        return Status.NOT_IMPLEMENTED
    
    def _start(self) -> Status:
        """
        Start the client (validate socket directory).
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.state > State.NULL:
            log("{}: Start in wrong state: {}", name(self), self.state.name)
            return Status.WRONG_STATE
        
        sock_dir = self.cfg.sock_dir
        if not os.path.isdir(sock_dir):
            log("{}: Socket dir '{}' is not dir or doesn't exist", name(self), sock_dir)
            return Status.ERROR
        
        self.sock_path = os.path.join(sock_dir, "a2e.sock")
        return Status.OK
    
    def _stop(self) -> Status:
        """
        Stop the client.
        
        Returns:
            Status.OK
        """
        log("{}: Client stop in state {}", name(self), self.state.name)
        
        if self.fd is not None:
            try:
                self.fd.close()
            except:
                pass
            self.fd = None
        
        self._cleanup()
        set_state(self, State.NULL)
        return Status.OK
    
    def _connect(self) -> Status:
        """
        Connect to the server.
        
        Returns:
            Status.CONTINUE on success, error status otherwise
        """
        try:
            self.fd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            self.fd.setblocking(False)
            
            self.fd.connect(self.sock_path)
            set_state(self, State.REQ_TX_START)
            return Status.CONTINUE
            
        except BlockingIOError:
            # Connection in progress - will be handled by poll
            set_state(self, State.REQ_TX_START)
            return Status.CONTINUE
            
        except Exception as e:
            log("{}: Failed to connect ({})", name(self), str(e))
            return Status.ERROR
    
    def _read_start(self, timeout_ms: int) -> Status:
        """
        Start reading response header.
        
        Returns:
            Status.CONTINUE on success, Status.IN_PROGRESS for progress,
            error status otherwise
        """
        if self.fd is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50  # DEF_A2E_RW_POLL_TIMEOUT_MS
        
        while True:
            try:
                ready, _, _ = select.select([self.fd], [], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    try:
                        header_data = self.fd.recv(MSG_HEADER_SIZE)
                    except Exception as e:
                        log("{}: Read error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    if len(header_data) < MSG_HEADER_SIZE:
                        log("{}: Incomplete header received", name(self))
                        return Status.ERROR
                    
                    msg = A2EMessage.unpack(header_data)
                    
                    if not msg.is_valid():
                        log("{}: Malformed message received (invalid magic)", name(self))
                        return Status.ERROR
                    
                    if msg.type == MsgType.PROGRESS:
                        log("{}: RX Progress msg", name(self))
                        set_state(self, State.REQ_PROGRESS)
                        return Status.IN_PROGRESS
                    
                    self.rsp_size_exp = msg.len
                    self.rsp_size_recv = 0
                    self.rsp = bytearray(msg.len)
                    
                    log("{}: RX Response msg (size: {})", name(self), self.rsp_size_exp)
                    set_state(self, State.RSP_RX)
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Read start error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.TIMEOUT
    
    def _read(self, timeout_ms: int) -> Status:
        """
        Read response data.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.fd is None or self.rsp is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        while True:
            try:
                ready, _, _ = select.select([self.fd], [], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    remaining = self.rsp_size_exp - self.rsp_size_recv
                    chunk_size = min(self.cfg.rw_chunk_size, remaining)
                    
                    try:
                        data = self.fd.recv(chunk_size)
                    except Exception as e:
                        log("{}: Read error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    if not data:
                        log("{}: Connection closed by remote side", name(self))
                        return Status.ERROR
                    
                    self.rsp[self.rsp_size_recv:self.rsp_size_recv + len(data)] = data
                    self.rsp_size_recv += len(data)
                    
                    log("{}: Recv data chunk size[{}] (total: {}/{})",
                        name(self), len(data), self.rsp_size_recv, self.rsp_size_exp)
                    
                    if self.rsp_size_recv >= self.rsp_size_exp:
                        set_state(self, State.RSP_RX_FINISH)
                        return Status.OK
                    
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Read error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.CONTINUE_TIMEOUT
    
    def _write_start(self, data: bytes, timeout_ms: int) -> Status:
        """
        Start writing request header.
        
        Returns:
            Status.CONTINUE on success, error status otherwise
        """
        if self.fd is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        msg = A2EMessage.create_request(len(data))
        
        while True:
            try:
                _, ready, _ = select.select([], [self.fd], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    try:
                        sent = self.fd.send(msg.pack())
                    except Exception as e:
                        log("{}: Write error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    if sent < MSG_HEADER_SIZE:
                        log("{}: Failed to send complete header", name(self))
                        return Status.ERROR
                    
                    self.req_size_sent = 0
                    self._req_data = data
                    
                    log("{}: TX Request msg (size: {})", name(self), len(data))
                    set_state(self, State.REQ_TX)
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Write start error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.TIMEOUT
    
    def _write(self, data: bytes, timeout_ms: int) -> Status:
        """
        Write request data.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.fd is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        while True:
            try:
                _, ready, _ = select.select([], [self.fd], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    remaining = len(data) - self.req_size_sent
                    chunk_size = min(self.cfg.rw_chunk_size, remaining)
                    offset = self.req_size_sent
                    
                    try:
                        sent = self.fd.send(data[offset:offset + chunk_size])
                    except Exception as e:
                        log("{}: Write error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    self.req_size_sent += sent
                    
                    log("{}: Sent data chunk size[{}] (total: {}/{})",
                        name(self), sent, self.req_size_sent, len(data))
                    
                    if self.req_size_sent >= len(data):
                        set_state(self, State.REQ_TX_FINISH)
                        return Status.OK
                    
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Write error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.CONTINUE_TIMEOUT
    
    def _cleanup(self) -> None:
        """Clean up request/response state"""
        if self.fd is not None:
            try:
                self.fd.close()
            except:
                pass
            self.fd = None
        
        self.rsp = None
        self.rsp_size_recv = 0
        self.rsp_size_exp = 0
        self.req_size_sent = 0
        self._req_data = None
