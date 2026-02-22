"""
A2E Library - Message

Message format and serialization for A2E library.
"""

import struct
from dataclasses import dataclass
from typing import Optional

from .defs import MsgType, A2E_MSG_MAGIC, A2E_MSG_LEN_MAX


# Message header format: type (4 bytes) + magic (4 bytes) + len (4 bytes)
# '<' = little-endian, 'I' = unsigned int (4 bytes)
MSG_HEADER_FORMAT = '<III'
MSG_HEADER_SIZE = struct.calcsize(MSG_HEADER_FORMAT)


@dataclass
class A2EMessage:
    """
    A2E message structure.
    
    This structure is a header of real message that will be sent or received.
    The header is sent first, followed by opaque message body of specified length.
    """
    
    type: MsgType
    """Type to define the message"""
    
    magic: int = A2E_MSG_MAGIC
    """Magic for validation"""
    
    len: int = 0
    """Message length (payload size)"""
    
    def __post_init__(self):
        if self.len > A2E_MSG_LEN_MAX:
            raise ValueError(f"Message length exceeds maximum: {self.len} > {A2E_MSG_LEN_MAX}")
    
    def pack(self) -> bytes:
        """Serialize message header to bytes"""
        return struct.pack(MSG_HEADER_FORMAT, self.type, self.magic, self.len)
    
    @classmethod
    def unpack(cls, data: bytes) -> 'A2EMessage':
        """Deserialize message header from bytes"""
        if len(data) < MSG_HEADER_SIZE:
            raise ValueError(f"Insufficient data for message header: {len(data)} < {MSG_HEADER_SIZE}")
        
        msg_type, magic, length = struct.unpack(MSG_HEADER_FORMAT, data[:MSG_HEADER_SIZE])
        return cls(type=MsgType(msg_type), magic=magic, len=length)
    
    def is_valid(self) -> bool:
        """Check if message magic is valid"""
        return self.magic == A2E_MSG_MAGIC
    
    @classmethod
    def create_request(cls, length: int) -> 'A2EMessage':
        """Create a request message with specified payload length"""
        return cls(type=MsgType.REQUEST, magic=A2E_MSG_MAGIC, len=length)
    
    @classmethod
    def create_response(cls, length: int) -> 'A2EMessage':
        """Create a response message with specified payload length"""
        return cls(type=MsgType.RESPONSE, magic=A2E_MSG_MAGIC, len=length)
    
    @classmethod
    def create_progress(cls) -> 'A2EMessage':
        """Create a progress message (always has zero length)"""
        return cls(type=MsgType.PROGRESS, magic=A2E_MSG_MAGIC, len=0)


def fill_request(msg: A2EMessage, length: int) -> None:
    """Fill message with request data"""
    msg.type = MsgType.REQUEST
    msg.magic = A2E_MSG_MAGIC
    msg.len = length


def fill_response(msg: A2EMessage, length: int) -> None:
    """Fill message with response data"""
    msg.type = MsgType.RESPONSE
    msg.magic = A2E_MSG_MAGIC
    msg.len = length


def fill_progress(msg: A2EMessage) -> None:
    """Fill message with progress data"""
    msg.type = MsgType.PROGRESS
    msg.magic = A2E_MSG_MAGIC
    msg.len = 0
