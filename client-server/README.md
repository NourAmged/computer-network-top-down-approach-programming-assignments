# Client-Server TCP Demo

A small C-based TCP client-server example that demonstrates a simple request/response protocol using `struct` messages over a socket connection.

## Overview

- The server listens on `localhost:8080` for a single TCP client connection.
- The client connects to the server and repeatedly sends a number.
- For each request, the server generates a random number between `1` and `100`, adds it to the client’s number, and sends the result back.
- If the client sends a number above `100`, the client and server exit after the final exchange.

## Files

- `server.c` - TCP server implementation.
- `client.c` - TCP client implementation.



## Build

From the repository root:

```bash
make
```

This creates two executables:

- `server`
- `client`

## Run

1. Start the server in one terminal:

```bash
./server
```

2. Start the client in another terminal:

```bash
./client
```

## Usage

- The client prompts:

```text
pick number between 1 and 100 (above 100 will send final request before ending)
```

- Enter a number between `1` and `100` to continue the exchange.
- Enter a number greater than `100` to send the final request and terminate the program.

## Protocol

The client and server use a fixed-size message structure:

```c
struct client_message {
    int number;
    char message[256];
};

struct server_message {
    int number;
    char message[256];
};
```

- Client sends `client_message` containing a number and a text message.
- Server responds with `server_message` containing the sum and a text message.

## Notes

- This implementation uses `127.0.0.1` and port `8080`.
- The server accepts only one client connection and handles requests in a simple loop.
- The random number is regenerated for each client request.

## Cleanup

```bash
make clean
```
