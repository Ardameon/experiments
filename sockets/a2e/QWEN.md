# A2E Library — Project Context

## Project Overview

**A2E** (Access mediator to ESR OI-manager) is a transport library for inter-process communication via **UNIX domain stream sockets**. The library implements a client-server architecture supporting request-response transactions with optional progress notifications.

### Key Features

- **Client-Server Architecture**: Simple UNIX stream socket implementation
- **Request-Response Transactions**: Structured message exchange protocol
- **Progress Notifications**: Server can send progress updates during request processing
- **Chunked I/O**: Data is transferred in configurable chunks (default 64KB)
- **Non-blocking I/O**: All operations support timeouts to avoid blocking
- **State Machine**: Well-defined state transitions for both client and server
- **Dual Implementation**: Original C library + Python port

### Transaction Flow

```
Client                          Server
  │                               │
  │──────── connect() ───────────▶│
  │                               │
  │──────── REQUEST (data) ──────▶│
  │                               │
  │◄─────── PROGRESS (optional) ──│
  │                               │
  │◄─────── RESPONSE (data) ──────│
  │                               │
  │──────── disconnect() ─────────▶│
  │                               │
```

### Message Format

Each message consists of a **12-byte header** + **payload**:

| Field | Size | Value |
|-------|------|-------|
| type | 4 bytes | REQUEST (0x1), RESPONSE (0x2), PROGRESS (0x3) |
| magic | 4 bytes | 0xBADA77BC |
| len | 4 bytes | Payload size |

## Directory Structure

```
a2e/
├── CMakeLists.txt          # Main CMake build configuration
├── server.c                # Server example application
├── client.c                # Client example application
├── include/                # Public header files
│   ├── a2e.h               # Main public API
│   ├── a2e_def.h           # Type definitions, enums, config
│   ├── a2e_common.h        # Internal common utilities
│   ├── a2e_dbg.h           # Debug/logging interface
│   ├── a2e_msg.h           # Message format definitions
│   ├── a2e_iface.h         # Strategy interface definition
│   ├── a2e_server.h        # Server internal structure
│   └── a2e_client.h        # Client internal structure
├── src/                    # Implementation files
│   ├── a2e.c               # Main library entry point
│   ├── a2e_server.c        # Server implementation
│   ├── a2e_client.c        # Client implementation
│   ├── a2e_msg.c           # Message encoding/decoding
│   ├── a2e_common.c        # Common utilities
│   └── a2e_dbg.c           # Debug logging implementation
├── tests/                  # Unit tests (CppUTest framework)
│   ├── CMakeLists.txt      # Test build configuration
│   ├── cpputest/           # CppUTest submodule
│   ├── test_a2e.cpp        # Core library tests
│   ├── test_a2e_client.cpp # Client tests
│   ├── test_a2e_common.cpp # Common utilities tests
│   ├── test_a2e_dbg.cpp    # Debug logging tests
│   ├── test_a2e_msg.cpp    # Message tests
│   └── test_main.cpp       # Test runner entry point
├── python_imp/             # Python implementation
│   ├── README.md           # Python-specific documentation
│   ├── a2e/                # Python package
│   │   ├── __init__.py     # Public API exports
│   │   ├── defs.py         # Constants, enums, config
│   │   ├── exceptions.py   # Exception classes
│   │   ├── msg.py          # Message format handling
│   │   ├── dbg.py          # Logging utilities
│   │   ├── common.py       # Common functions
│   │   ├── iface.py        # Base interface
│   │   ├── client.py       # Client implementation
│   │   └── server.py       # Server implementation
│   ├── demo_server.py      # Python server demo
│   └── demo_client.py      # Python client demo
└── build/                  # CMake build output (git-ignored)
    ├── liba2e.a            # Static library
    ├── liba2e.so           # Shared library
    ├── server              # Server executable
    ├── client              # Client executable
    └── tests/              # Test executables
```

## Building and Running

### Prerequisites

- CMake 3.10+
- GCC/G++ with C11/C++11 support
- CppUTest (included as submodule for tests)

### Build Commands

```bash
# Navigate to project directory
cd /home/pylesosik/my/programming/experiments/sockets/a2e

# Create build directory and configure
mkdir -p build && cd build
cmake ..

# Build libraries and executables
make

# Build and run tests
make tests
ctest --output-on-failure

# Install to local directory
make install
# Installed to: build/install/bin/{server,client}, build/install/lib/{liba2e.a,liba2e.so}
```

### Running Examples

**Terminal 1 (Server):**
```bash
cd build
./server
```

**Terminal 2 (Client):**
```bash
cd build
./client
```

### Testing with Netcat

```bash
# Start server
./build/server

# Connect with netcat (in another terminal)
nc -U /tmp/a2e.sock
```

### Python Implementation

```bash
cd python_imp

# Start server
python3 demo_server.py

# Start client (in another terminal)
python3 demo_client.py
```

## API Reference

### Core Types

```c
// Status codes
typedef enum {
    eA2E_SC_OK,              // Success
    eA2E_SC_IN_PROGRESS,     // Progress response received
    eA2E_SC_TIMEOUT,         // Timeout
    eA2E_SC_CONTINUE,        // Continue operation
    eA2E_SC_CONTINUE_TIMEOUT,// Timeout during continue
    eA2E_SC_NOT_IMPLEMENTED, // Function not implemented
    eA2E_SC_INVALID_ARGS,    // Invalid arguments
    eA2E_SC_NO_MEM,          // Memory allocation failed
    eA2E_SC_WRONG_STATE,     // Invalid state for operation
    eA2E_SC_INCOMPLETE_READ, // Incomplete data read
    eA2E_SC_ERROR,           // General error
} a2e_status_e;

// Configuration structure
typedef struct a2e_cfg_t {
    char name[A2E_NAME_LEN_MAX];       // Instance name for logs
    char sock_dir[A2E_SOCK_DIR_LEN_MAX]; // Socket directory
    unsigned rw_chunk_size;            // RW chunk size (default: 64KB)
    unsigned req_cmplt_wait_tries;     // Wait tries for request complete
} a2e_cfg_t;
```

### Public Functions

| Function | Description |
|----------|-------------|
| `a2e_init_client(&a2e, &cfg)` | Initialize client instance |
| `a2e_init_server(&a2e, &cfg)` | Initialize server instance |
| `a2e_close(a2e)` | Close and cleanup instance |
| `a2e_request_rx(a2e, &buf, &size, to_ms)` | Receive request (server) |
| `a2e_request_tx(a2e, buf, size, to_ms)` | Send request (client) |
| `a2e_request_complete(a2e)` | Complete request (client) |
| `a2e_request_complete_wait(a2e, to_ms)` | Wait for request complete (server) |
| `a2e_response_rx(a2e, &buf, &size, to_ms)` | Receive response (client) |
| `a2e_response_tx(a2e, buf, size, to_ms)` | Send response (server) |
| `a2e_progress_tx(a2e, to_ms)` | Send progress notification (server) |
| `a2e_cfg_set_default(&cfg)` | Set default configuration |
| `a2e_dbg_on()` / `a2e_dbg_off()` | Enable/disable debug logs |
| `a2e_dbg_set_func(log_func)` | Set custom log function |
| `a2e_perror(status)` | Get status string |

### State Machine

**Server States:**
```
NULL → IDLE → REQ_RX_START → REQ_RX → REQ_RX_FINISH
                    → REQ_PROGRESS (optional)
                    → RSP_TX → RSP_TX_FINISH → IDLE
```

**Client States:**
```
NULL → IDLE → REQ_TX_START → REQ_TX → REQ_TX_FINISH
                    → REQ_PROGRESS (receive)
                    → RSP_RX → RSP_RX_FINISH → IDLE
```

## Development Practices

### Code Style

- **C Standard**: C11 with POSIX extensions
- **Naming Convention**: 
  - Types: `snake_case_t` suffix
  - Enums: `eA2E_*` prefix
  - Functions: `a2e_*` prefix
  - Macros: `A2E_*` or `DEF_A2E_*` prefix
- **File Organization**: Headers in `include/`, implementation in `src/`
- **Documentation**: Doxygen-style comments with `@brief`, `@param`, `@return`

### Testing

- **Framework**: CppUTest (C++ testing framework for C code)
- **Coverage**: GCC `--coverage` flags enabled for test targets
- **Test Targets**: `test_common`, `test_a2e`, `test_a2e_client`
- **Generate Coverage Report**: `ctest -T Coverage`

### Debugging

```c
// Enable debug logging
a2e_dbg_on();

// Set custom log function
a2e_dbg_set_func(my_log_function);

// Log function signature
void my_log_function(const char *log_str, int len);
```

## Limitations

| Limitation | Description |
|------------|-------------|
| Single Connection | Server handles one client at a time |
| UNIX Only | Requires UNIX domain sockets (Linux, macOS) |
| No Authentication | Any process can connect to socket |
| No Encryption | Data transmitted in plaintext |
| Synchronous I/O | No async/asyncio support |
| No Connection Pool | No thread/process pool for concurrency |

## Known Issues

1. **Race Conditions**: Multiple simultaneous clients may cause connection issues
2. **Socket Leaks**: Abnormal client termination may leave sockets in `CLOSE_WAIT`
3. **Large Data Blocking**: Transfers >100MB may block for extended periods
4. **No Access Control**: Socket permissions not restricted by default
5. **MSG_PEEK Performance**: `recv(1, MSG_PEEK)` in `req_cmplt_wait()` may impact performance

## Python Implementation Notes

The Python implementation in `python_imp/` is a **functional prototype** that mirrors the C library API:

- **Generated**: February 2026 by Qwen Code AI
- **Status**: Requires testing before production use
- **Compatibility**: Protocol-compatible with C implementation
- **Missing**: Unit tests, performance optimization, production hardening

See `python_imp/README.md` for detailed Python documentation.

## Quick Reference

```bash
# Build everything
cd build && cmake .. && make

# Run tests
make tests && ctest --output-on-failure

# Run server example
./build/server

# Run client example  
./build/client

# Generate coverage report
ctest -T Coverage

# Install locally
make install  # to build/install directory
```
