#!/usr/bin/env python3
"""
A2E Library - Demo Server

Demonstration server implementation similar to server.c
"""

import time
import sys
import os

# Add parent directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from a2e import *

TO = 500  # Timeout in milliseconds


def log_fun(msg: str) -> None:
    """Custom log function"""
    print(f"[A2E_TEST_SERVER] {msg}")


def main() -> int:
    """Main server function"""
    requests_cnt = 100

    # Enable debug logging
    dbg_on()
    dbg_set_func(log_fun)

    # Create configuration
    cfg = cfg_set_default()
    cfg.name = "SERVER_NAME"

    # For tests use netcat client:
    # $ nc -U /tmp/a2e.sock

    # Initialize server
    server = init_server(cfg)

    try:
        for i in range(requests_cnt):
            wait_cnt = 0
            wait_limit = 10
            
            # Receive request
            while True:
                status, buf_rx = request_rx(server, TO)
                print(f"AAAAA. RX status: {perror(status)}")

                if status == Status.OK:
                    break

                if status not in (Status.TIMEOUT, Status.CONTINUE, Status.CONTINUE_TIMEOUT):
                    break

            if status == Status.OK:
                # Send progress notification
                progress_tx(server, TO)

                # Imitate some progress
                time.sleep(2)

                # Send response
                while True:
                    status = response_tx(server, buf_rx, TO)
                    print(f"AAAAA. TX status: {perror(status)}")

                    if status == Status.OK:
                        break

                    if status not in (Status.TIMEOUT, Status.CONTINUE, Status.CONTINUE_TIMEOUT):
                        break

            # Wait for request complete
            while True:
                status = request_complete_wait(server, TO)

                if status != Status.OK:
                    wait_cnt += 1
                    if wait_cnt < wait_limit:
                        continue

                print(f"AAAAA. Wait limit reached: {perror(status)}")
                break

    except KeyboardInterrupt:
        print("\nServer interrupted")
    except Exception as e:
        print(f"Server error: {e}")
        return 1
    finally:
        close(server)

    return 0


if __name__ == "__main__":
    sys.exit(main())
