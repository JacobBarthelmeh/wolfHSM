# TLS/DTLS Server with wolfHSM Crypto Offload

This example demonstrates a TLS/DTLS server that offloads cryptographic
operations to a wolfHSM server. By default, DTLS (UDP-based) is used, but
the code can be adapted for TLS (TCP-based) connections.

The wolfHSM server runs separately and communicates via shared memory with
DMA support for efficient data transfer.

## Architecture

```
+-------------------+       Shared Memory (DMA)       +-------------------+
|                   | <-----------------------------> |                   |
|  TLS/DTLS Server  |   Crypto Operations Request    |  wolfHSM Server   |
|  (This Example)   | <-----------------------------> |  (wh_posix_server)|
|                   |   Crypto Operations Response   |                   |
+-------------------+                                 +-------------------+
        |                                                      |
        | TLS/DTLS                                             | Performs all
        |                                                      | crypto ops:
        v                                                      | - Key Exchange
+-------------------+                                          | - Signing
|  TLS/DTLS Client  |                                          | - Encryption
|                   |                                          | - Hashing
+-------------------+                                          +
```

## How It Works

1. **wolfHSM Server**: The `wh_posix_server` runs with `--type dma` to provide
   crypto services over shared memory with DMA support.

2. **TLS/DTLS Server**: This example connects to the wolfHSM server as a client
   and registers a crypto callback. All wolfSSL/wolfCrypt operations are
   forwarded to the HSM.

3. **Crypto Offload**: When `wolfSSL_SetDevId()` is called with `WH_DEV_ID`,
   wolfSSL routes crypto operations through the registered callback to the
   wolfHSM server.

## Building

### Prerequisites

1. wolfSSL library built with crypto callback support
2. wolfHSM library with POSIX port

### Build Steps

```bash
# Build the server
cd examples/demo/dtls_server
make DEBUG=1

# Build the wolfHSM POSIX server (if not already built)
cd ../../posix/wh_posix_server
make DEBUG=1
```

## Running

### Step 1: Start the wolfHSM Server

```bash
cd examples/posix/wh_posix_server
./Build/wh_posix_server.elf --type dma
```

You should see:
```
Example wolfHSM POSIX server built with wolfSSL version X.X.X
Using DMA with shared memory transport
Waiting for connection...
```

### Step 2: Start the Server

In a new terminal:

```bash
cd examples/demo/dtls_server
./Build/wh_server.elf
```

#### Command-Line Options

```
Usage: ./Build/wh_server.elf [options]

Options:
  -A <file>    CA certificate file (PEM or DER format)
               If not specified, uses built-in test certificate
  -c <file>    Server certificate file (PEM or DER format)
  -k <file>    Server private key file (PEM or DER format)
  -p <port>    Port to listen on (default: 11111)
  -h           Show this help message
```

#### Examples

```bash
# Use default built-in certificates
./Build/wh_server.elf

# Use custom CA certificate
./Build/wh_server.elf -A /path/to/ca-cert.pem

# Use custom certificates and port
./Build/wh_server.elf -A ca.pem -c server.pem -k server-key.pem -p 4433
```

You should see:
```
===========================================
wolfHSM Server Demo (DTLS mode)
===========================================

This server offloads crypto operations to
the wolfHSM server running with --type dma

Connecting to wolfHSM server via shared memory DMA...
Registered WH_DEV_ID (0x5748534D) crypto callback
Registered WH_DEV_ID_DMA (0x57444D41) crypto callback
Connected to wolfHSM server successfully

Waiting for client on port 11111...
```

### Step 3: Connect a Client

In a third terminal:

```bash
# For DTLS (default mode) - using openssl with DTLS 1.2
openssl s_client -dtls1_2 -connect localhost:11111

# Or use the wolfSSL example client with DTLS
./examples/client/client -u -v 3 -p 11111
```

## Key Integration Points

### 1. Registering Crypto Callbacks

In `server.c`, we register the wolfHSM crypto callbacks:

```c
/* Register crypto callback for non-DMA operations */
wc_CryptoCb_RegisterDevice(WH_DEV_ID, wh_Client_CryptoCb, (void*)g_client);

/* Register crypto callback for DMA operations (larger data) */
wc_CryptoCb_RegisterDevice(WH_DEV_ID_DMA, wh_Client_CryptoCbDma, (void*)g_client);
```

### 2. Setting Device ID on wolfSSL Object

In `server_io.c`, we configure wolfSSL to use the HSM on the SSL object
after creation (to allow local RNG initialization):

```c
/* Set device ID after wolfSSL_new() for crypto offload */
wolfSSL_SetDevId(ssl, WH_DEV_ID);
```

### 3. DMA vs Non-DMA

- **WH_DEV_ID**: Uses standard message-based crypto operations. Suitable for
  smaller data sizes.
  
- **WH_DEV_ID_DMA**: Uses DMA (Direct Memory Access) for data transfer.
  More efficient for larger data like TLS record encryption.

## Configuration

### wolfSSL Configuration (`config/user_settings.h`)

Key settings for TLS/DTLS with wolfHSM:

```c
/* Enable DTLS (for UDP mode) */
#define WOLFSSL_DTLS

/* Enable crypto callbacks for wolfHSM */
#define WOLF_CRYPTO_CB

/* Hash DRBG for RNG */
#define HAVE_HASHDRBG
```

### wolfHSM Configuration (`config/wolfhsm_cfg.h`)

Key settings for wolfHSM client:

```c
/* Enable wolfHSM client */
#define WOLFHSM_CFG_ENABLE_CLIENT

/* Enable DMA transport */
#define WOLFHSM_CFG_DMA
```

## Troubleshooting

### "Failed to connect to wolfHSM server"

Make sure the `wh_posix_server` is running with `--type dma` before starting
the DTLS server.

### Handshake Failures

1. Check that both the wolfHSM server and DTLS server are built with the same
   wolfSSL version.
   
2. Verify the shared memory name matches between client and server
   (default: "wh_example_shm").

3. Enable verbose debugging:
   ```bash
   make DEBUG_VERBOSE=1
   ```

### Permission Issues

If you see shared memory permission errors, you may need to clean up stale
shared memory segments:

```bash
# List shared memory segments
ls -la /dev/shm/

# Remove stale segment (if present)
rm /dev/shm/wh_example_shm
```

## Adapting for TLS

To use TLS instead of DTLS:

1. In `server_io.c`, change the method from `wolfDTLSv1_2_server_method()` to
   `wolfTLSv1_3_server_method()` or another TLS method.

2. Change the socket initialization from UDP to TCP (replace
   `initialize_udp_socket()` with a TCP equivalent).

3. Remove the DTLS-specific peer address setup in `setup_ssl_accept()`.

## Files

- `server.c` - Main entry point, wolfHSM client setup
- `server_io.c` - TLS/DTLS I/O implementation
- `server.h` - Public API header
- `config/user_settings.h` - wolfSSL configuration
- `config/wolfhsm_cfg.h` - wolfHSM configuration
- `Makefile` - Build configuration
