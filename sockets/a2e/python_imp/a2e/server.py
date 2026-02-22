"""
A2E Library - Server

UNIX domain socket server implementation for A2E library.
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


class A2EServer(A2EInstance):
    """
    A2E server implementation.
    
    Listens for client connections via UNIX domain socket and handles
    REQUEST-RESPONSE transactions.
    """
    
    def __init__(self):
        super().__init__()
        self.role = Role.SERVER
        self.state = State.NULL
        
        self.sock_path: str = ""
        """Instance socket path"""
        
        self.fd_listen: Optional[socket.socket] = None
        """Listen socket file descriptor"""
        
        self.fd_rw: Optional[socket.socket] = None
        """Read/Write socket file descriptor"""
        
        self.req: Optional[bytes] = None
        """Received request buffer"""
        
        self.req_size_recv: int = 0
        """Size of received request"""
        
        self.req_size_exp: int = 0
        """Expected size of request"""
        
        self.req_cmplt_wait_tries: int = 0
        """Request complete wait tries counter"""
        
        self.rsp_size_sent: int = 0
        """Size of sent response"""
    
    def init(self, cfg: A2EConfig) -> Status:
        """
        Initialize the A2E server.
        
        Args:
            cfg: Configuration for the server
            
        Returns:
            Status.OK on success, error status otherwise
        """
        if cfg is None:
            return Status.INVALID_ARGS
        
        cfg_apply(self, cfg)
        self.role = Role.SERVER
        self.state = State.NULL
        
        status = self._start()
        if status != Status.OK:
            return status
        
        set_state(self, State.IDLE)
        log("{}: Server initialized (unix: '{}')", name(self), self.cfg.sock_dir)
        return Status.OK
    
    def close(self) -> Status:
        """
        Close the A2E server.
        
        Returns:
            Status.OK on success
        """
        self._stop()
        return Status.OK
    
    def req_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """
        Receive a request from a client.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Tuple of (status, received data or None)
        """
        if self.state == State.IDLE:
            status = self._accept(timeout_ms)
            return status, None
        
        if self.state == State.REQ_RX_START:
            status = self._read_start(timeout_ms)
            return status, None
        
        if self.state == State.REQ_RX:
            status = self._read(timeout_ms)
            if status == Status.OK:
                return status, self.req
            return status, None
        
        return Status.WRONG_STATE, None
    
    def req_tx(self, data: bytes, timeout_ms: int) -> Status:
        """
        Transmit a request (not implemented for server).
        
        Args:
            data: Data to send
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.NOT_IMPLEMENTED
        """
        return Status.NOT_IMPLEMENTED
    
    def req_cmplt(self) -> Status:
        """
        Complete the current request.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.state in (State.REQ_RX_START, State.REQ_RX, State.REQ_RX_FINISH,
                          State.REQ_PROGRESS, State.RSP_TX, State.RSP_TX_FINISH):
            self._request_close()
            return Status.OK
        
        return Status.WRONG_STATE
    
    def req_cmplt_wait(self, timeout_ms: int) -> Status:
        """
        Wait for the client to complete the request.

        Args:
            timeout_ms: Timeout in milliseconds

        Returns:
            Status.OK on success, error status otherwise
        """
        if self.state != State.RSP_TX_FINISH:
            return Status.WRONG_STATE

        if self.req_cmplt_wait_tries >= self.cfg.req_cmplt_wait_tries:
            self._request_close()
            return Status.OK

        if self.fd_rw is None:
            self._request_close()
            return Status.OK

        try:
            ready, _, exceptional = select.select([self.fd_rw], [], [self.fd_rw], timeout_ms / 1000.0)

            if exceptional:
                # Exceptional condition on socket - client disconnected
                self._request_close()
                return Status.OK

            if ready:
                # Check if there's actual data to read or if connection was closed
                # Try to peek at the data without consuming it
                try:
                    # Save original socket flags
                    original_flags = self.fd_rw.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR)
                    if original_flags != 0:
                        # Socket error - client disconnected
                        self._request_close()
                        return Status.OK
                    
                    # Try to read 1 byte with MSG_PEEK to check if data is available
                    # If recv returns empty bytes, connection was closed gracefully
                    self.fd_rw.setblocking(False)
                    data = self.fd_rw.recv(1, socket.MSG_PEEK)
                    if not data:
                        # Connection closed gracefully by client
                        self._request_close()
                        return Status.OK
                    else:
                        # There's data to read - client is sending more than expected
                        return Status.INCOMPLETE_READ
                except BlockingIOError:
                    # No data available - this is expected, client is idle
                    # Timeout - increment tries
                    self.req_cmplt_wait_tries += 1
                    return Status.TIMEOUT
                except ConnectionResetError:
                    # Connection reset by client
                    self._request_close()
                    return Status.OK
                except Exception:
                    # Any other error - assume client disconnected
                    self._request_close()
                    return Status.OK

            # Timeout - increment tries
            self.req_cmplt_wait_tries += 1
            return Status.TIMEOUT

        except Exception:
            self._request_close()
            return Status.OK
    
    def resp_rx(self, timeout_ms: int) -> Tuple[Status, Optional[bytes]]:
        """
        Receive a response (not implemented for server).
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.NOT_IMPLEMENTED, None
        """
        return Status.NOT_IMPLEMENTED, None
    
    def resp_tx(self, data: bytes, timeout_ms: int) -> Status:
        """
        Transmit a response to the client.
        
        Args:
            data: Data to send
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        if data is None:
            return Status.INVALID_ARGS
        
        if self.state in (State.REQ_RX_FINISH, State.REQ_PROGRESS):
            return self._write_start(data, timeout_ms)
        
        if self.state == State.RSP_TX:
            return self._write(data, timeout_ms)
        
        return Status.WRONG_STATE
    
    def prog_tx(self, timeout_ms: int) -> Status:
        """
        Transmit a progress notification to the client.
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.state in (State.REQ_RX_FINISH, State.REQ_PROGRESS):
            return self._progress(timeout_ms)
        
        return Status.WRONG_STATE
    
    def _start(self) -> Status:
        """
        Start the server (create listen socket).
        
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
        
        # Remove existing socket file
        try:
            if os.path.exists(self.sock_path):
                os.unlink(self.sock_path)
        except Exception as e:
            log("{}: Failed to remove existing socket: {}", name(self), str(e))
        
        try:
            self.fd_listen = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            self.fd_listen.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.fd_listen.setblocking(False)
            self.fd_listen.bind(self.sock_path)
            self.fd_listen.listen(1)
        except Exception as e:
            log("{}: Failed to create/bind socket ({})", name(self), str(e))
            return Status.ERROR
        
        return Status.OK
    
    def _stop(self) -> Status:
        """
        Stop the server.
        
        Returns:
            Status.OK
        """
        log("{}: Server stop in state {}", name(self), self.state.name)
        
        if self.fd_listen is not None:
            try:
                self.fd_listen.close()
            except:
                pass
            self.fd_listen = None
        
        self._request_close()
        
        # Remove socket file
        try:
            if os.path.exists(self.sock_path):
                os.unlink(self.sock_path)
        except:
            pass
        
        set_state(self, State.NULL)
        return Status.OK
    
    def _accept(self, timeout_ms: int) -> Status:
        """
        Accept a client connection.
        
        Returns:
            Status.CONTINUE on success, Status.TIMEOUT on timeout
        """
        if self.fd_listen is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        
        while True:
            try:
                ready, _, _ = select.select([self.fd_listen], [], [], timeout_ms / 1000.0)
                
                if ready:
                    try:
                        self.fd_rw, _ = self.fd_listen.accept()
                        self.fd_rw.setblocking(False)
                        self.req_cmplt_wait_tries = 0
                        set_state(self, State.REQ_RX_START)
                        log("{}: Client connected", name(self))
                        return Status.CONTINUE
                    except Exception as e:
                        log("{}: Accept failed ({})", name(self), str(e))
                        return Status.ERROR
                        
            except Exception as e:
                log("{}: Accept error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.TIMEOUT
    
    def _read_start(self, timeout_ms: int) -> Status:
        """
        Start reading request header.
        
        Returns:
            Status.CONTINUE on success, error status otherwise
        """
        if self.fd_rw is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50  # DEF_A2E_RW_POLL_TIMEOUT_MS
        
        while True:
            try:
                ready, _, _ = select.select([self.fd_rw], [], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    try:
                        header_data = self.fd_rw.recv(MSG_HEADER_SIZE)
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
                    
                    self.req_size_exp = msg.len
                    self.req_size_recv = 0
                    self.req = bytearray(msg.len)
                    
                    log("{}: RX Request msg (size: {})", name(self), self.req_size_exp)
                    set_state(self, State.REQ_RX)
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Read start error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.TIMEOUT
    
    def _read(self, timeout_ms: int) -> Status:
        """
        Read request data.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.fd_rw is None or self.req is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        while True:
            try:
                ready, _, _ = select.select([self.fd_rw], [], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    remaining = self.req_size_exp - self.req_size_recv
                    chunk_size = min(self.cfg.rw_chunk_size, remaining)
                    
                    try:
                        data = self.fd_rw.recv(chunk_size)
                    except Exception as e:
                        log("{}: Read error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    if not data:
                        log("{}: Connection closed by remote side", name(self))
                        return Status.ERROR
                    
                    self.req[self.req_size_recv:self.req_size_recv + len(data)] = data
                    self.req_size_recv += len(data)
                    
                    log("{}: Recv data chunk size[{}] (total: {}/{})",
                        name(self), len(data), self.req_size_recv, self.req_size_exp)
                    
                    if self.req_size_recv >= self.req_size_exp:
                        set_state(self, State.REQ_RX_FINISH)
                        return Status.OK
                    
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Read error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.CONTINUE_TIMEOUT
    
    def _write_start(self, data: bytes, timeout_ms: int) -> Status:
        """
        Start writing response header.
        
        Returns:
            Status.CONTINUE on success, error status otherwise
        """
        if self.fd_rw is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        msg = A2EMessage.create_response(len(data))
        
        while True:
            try:
                _, ready, _ = select.select([], [self.fd_rw], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    try:
                        sent = self.fd_rw.send(msg.pack())
                    except Exception as e:
                        log("{}: Write error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    if sent < MSG_HEADER_SIZE:
                        log("{}: Failed to send complete header", name(self))
                        return Status.ERROR
                    
                    self.rsp_size_sent = 0
                    
                    log("{}: TX Response msg (size: {})", name(self), len(data))
                    set_state(self, State.RSP_TX)
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Write start error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.TIMEOUT
    
    def _write(self, data: bytes, timeout_ms: int) -> Status:
        """
        Write response data.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.fd_rw is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        while True:
            try:
                _, ready, _ = select.select([], [self.fd_rw], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    remaining = len(data) - self.rsp_size_sent
                    chunk_size = min(self.cfg.rw_chunk_size, remaining)
                    offset = self.rsp_size_sent
                    
                    try:
                        sent = self.fd_rw.send(data[offset:offset + chunk_size])
                    except Exception as e:
                        log("{}: Write error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    self.rsp_size_sent += sent
                    
                    log("{}: Sent data chunk size[{}] (total: {}/{})",
                        name(self), sent, self.rsp_size_sent, len(data))
                    
                    if self.rsp_size_sent >= len(data):
                        set_state(self, State.RSP_TX_FINISH)
                        return Status.OK
                    
                    return Status.CONTINUE
                    
            except Exception as e:
                log("{}: Write error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.CONTINUE_TIMEOUT
    
    def _progress(self, timeout_ms: int) -> Status:
        """
        Send progress notification.
        
        Returns:
            Status.OK on success, error status otherwise
        """
        if self.fd_rw is None:
            return Status.ERROR
        
        start_time = get_timestamp()
        poll_timeout_ms = 50
        
        msg = A2EMessage.create_progress()
        
        while True:
            try:
                _, ready, _ = select.select([], [self.fd_rw], [], poll_timeout_ms / 1000.0)
                
                if ready:
                    try:
                        sent = self.fd_rw.send(msg.pack())
                    except Exception as e:
                        log("{}: Progress write error ({})", name(self), str(e))
                        return Status.ERROR
                    
                    if sent < MSG_HEADER_SIZE:
                        log("{}: Failed to send complete progress header", name(self))
                        return Status.ERROR
                    
                    log("{}: TX Progress msg", name(self))
                    set_state(self, State.REQ_PROGRESS)
                    return Status.OK
                    
            except Exception as e:
                log("{}: Progress error ({})", name(self), str(e))
                return Status.ERROR
            
            if timeout_reached(start_time, timeout_ms):
                return Status.TIMEOUT
    
    def _request_close(self) -> Status:
        """
        Close active request and connection.
        
        Returns:
            Status.OK
        """
        if self.fd_rw is not None:
            try:
                self.fd_rw.close()
            except:
                pass
            self.fd_rw = None
        
        self.req = None
        self.req_size_recv = 0
        self.req_size_exp = 0
        self.req_cmplt_wait_tries = 0
        self.rsp_size_sent = 0
        
        set_state(self, State.IDLE)
        return Status.OK
