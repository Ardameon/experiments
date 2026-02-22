"""
A2E Library - Exceptions

Custom exceptions for A2E library.
"""

from .defs import Status


class A2EError(Exception):
    """Base exception for A2E library"""
    
    def __init__(self, status: Status = Status.ERROR, message: str = ""):
        self.status = status
        self.message = message or status.name
        super().__init__(self.message)


class A2ETimeout(A2EError):
    """Exception raised when a timeout occurs"""
    
    def __init__(self, message: str = "Timeout"):
        super().__init__(Status.TIMEOUT, message)


class A2EContinueTimeout(A2EError):
    """Exception raised when timeout occurs during continued operation"""
    
    def __init__(self, message: str = "Continue timeout"):
        super().__init__(Status.CONTINUE_TIMEOUT, message)


class A2EInvalidState(A2EError):
    """Exception raised when operation is not allowed in current state"""
    
    def __init__(self, message: str = "Invalid state"):
        super().__init__(Status.WRONG_STATE, message)


class A2EInvalidArgs(A2EError):
    """Exception raised when invalid arguments are provided"""
    
    def __init__(self, message: str = "Invalid arguments"):
        super().__init__(Status.INVALID_ARGS, message)


class A2ENoMemory(A2EError):
    """Exception raised when memory allocation fails"""
    
    def __init__(self, message: str = "No memory"):
        super().__init__(Status.NO_MEM, message)


class A2ENotImplemented(A2EError):
    """Exception raised when a function is not implemented"""
    
    def __init__(self, message: str = "Not implemented"):
        super().__init__(Status.NOT_IMPLEMENTED, message)


class A2EIncompleteRead(A2EError):
    """Exception raised when there is still data to read"""
    
    def __init__(self, message: str = "Incomplete read"):
        super().__init__(Status.INCOMPLETE_READ, message)


class A2EInProgress(A2EError):
    """Exception raised when progress response is received"""
    
    def __init__(self, message: str = "In progress"):
        super().__init__(Status.IN_PROGRESS, message)


class A2EContinue(A2EError):
    """Exception raised to signal that operation should continue"""
    
    def __init__(self, message: str = "Continue"):
        super().__init__(Status.CONTINUE, message)


class A2EGenericError(A2EError):
    """Generic error exception"""
    
    def __init__(self, message: str = "Error"):
        super().__init__(Status.ERROR, message)
