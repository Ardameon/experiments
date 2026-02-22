#!/usr/bin/env python3
"""
A2E Library - Demo Client

Demonstration client implementation similar to client.c
"""

import sys
import os

# Add parent directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from a2e import *

MSG_SIZE = 2 * 1024 * 1024  # 2 MB
TO = 500  # Timeout in milliseconds


def log_fun(msg: str) -> None:
    """Custom log function"""
    print(f"[A2E_TEST_CLIENT] {msg}")


def main() -> int:
    """Main client function"""
    requests_cnt = 1
    
    # Enable debug logging
    dbg_on()
    dbg_set_func(log_fun)
    
    # Create configuration
    cfg = cfg_set_default()
    cfg.name = "CLIENT_NAME"
    
    # Initialize client
    client = init_client(cfg)
    
    try:
        for i in range(requests_cnt):
            # Create request data
            buf_tx = bytes([i + ord('0')] * MSG_SIZE)
            
            # Send request
            while True:
                status = request_tx(client, buf_tx, TO)
                print(f"AAAAA. TX status: {perror(status)}")
                
                if status == Status.OK:
                    break
                
                if status not in (Status.TIMEOUT, Status.CONTINUE, Status.CONTINUE_TIMEOUT):
                    break
            
            if status == Status.OK:
                # Receive response
                while True:
                    status, buf_rx = response_rx(client, TO)
                    print(f"AAAAA. RX status: {perror(status)}")
                    
                    if status == Status.OK:
                        break
                    
                    if status == Status.IN_PROGRESS:
                        print(f"AAAAA. Progress received: {perror(status)}")
                        status = Status.CONTINUE
                        continue
                    
                    if status not in (Status.TIMEOUT, Status.CONTINUE, Status.CONTINUE_TIMEOUT):
                        break
            
            # Complete request
            request_complete(client)
    
    except KeyboardInterrupt:
        print("\nClient interrupted")
    except Exception as e:
        print(f"Client error: {e}")
        return 1
    finally:
        close(client)
    
    return 0


if __name__ == "__main__":
    sys.exit(main())
