# Simple C Web Server

A small HTTP web server written in C that listens on port 8080 and serves static HTML files from the current directory.

## What it does

This project demonstrates the basics of socket programming and HTTP response handling in C. The server:

- creates a TCP socket
- binds to port 8080
- listens for incoming connections
- reads the client request
- routes the request to a matching HTML file
- sends an HTTP response back to the client

## Supported routes

- `/` → serves `index.html`
- `/hello` → serves `hello.html`
- anything else → serves `error.html` with a 404 response

## Project files

- `main.c` — the server implementation
- `index.html` — homepage content
- `hello.html` — hello page content
- `error.html` — 404 page content

## Build

Make sure you have `gcc` installed, then compile the server from the project directory:

```bash
gcc -Wall -Wextra -std=c99 main.c -o main
```

## Run

Start the server:

```bash
./main
```

The server will print a message indicating that it is listening on port 8080.

## Test it

Open these URLs in your browser or use `curl`:

```bash
curl http://localhost:8080/
curl http://localhost:8080/hello
curl http://localhost:8080/unknown
```

## Notes

- The server is intentionally simple and educational.
- It handles basic `GET` requests.
- It serves static HTML files only.
