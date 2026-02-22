"""
A2E Library - Debug

Debug logging functionality for A2E library.
"""

from typing import Optional, Callable

# Type alias for log function
A2ELogFunc = Callable[[str], None]


class DebugConfig:
    """Debug configuration holder"""
    
    def __init__(self):
        self.log_func: Optional[A2ELogFunc] = None
        """External debug log function"""
        self.dbg_on: bool = False
        """Debug enabled flag"""


# Global debug configuration
_dbg_cfg = DebugConfig()


def a2e_log(format_str: str, *args, **kwargs) -> None:
    """
    Library log wrapper.
    
    This function will use external debug log function if it is set
    and print() in other case.
    
    Args:
        format_str: Log format string
        *args: Format arguments
        **kwargs: Additional format arguments
    """
    if not _dbg_cfg.dbg_on:
        return
    
    if args or kwargs:
        log_msg = format_str.format(*args, **kwargs)
    else:
        log_msg = format_str
    
    if _dbg_cfg.log_func:
        _dbg_cfg.log_func(log_msg)
    else:
        print(f"[A2E DBG] {log_msg}")


def dbg_on() -> None:
    """Turn debug logging on"""
    _dbg_cfg.dbg_on = True


def dbg_off() -> None:
    """Turn debug logging off"""
    _dbg_cfg.dbg_on = False


def dbg_set_func(log_func: Optional[A2ELogFunc]) -> None:
    """
    Set external debug log function.
    
    Args:
        log_func: Function to call for logging. Takes a single string argument.
                  If None, default print() will be used.
    """
    _dbg_cfg.log_func = log_func


def dbg_is_enabled() -> bool:
    """Check if debug logging is enabled"""
    return _dbg_cfg.dbg_on


def a2e_set_dbg(on: bool) -> None:
    """
    Set debug logging state.
    
    Args:
        on: True to enable, False to disable
    """
    _dbg_cfg.dbg_on = bool(on)


def a2e_set_dbg_log_func(log_func: Optional[A2ELogFunc]) -> None:
    """
    Set external debug log function (alias for dbg_set_func).
    
    Args:
        log_func: Function to call for logging
    """
    dbg_set_func(log_func)
