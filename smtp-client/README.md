# SMTP Client in C

A small C program that demonstrates how to connect to an SMTP server, upgrade the connection with STARTTLS, authenticate with Gmail, and send a simple email message using OpenSSL.

This project is intended for learning and experimentation with:

- TCP socket programming
- SMTP command flow
- STARTTLS and TLS negotiation
- base64 encoding for SMTP authentication
- OpenSSL APIs in C

## Overview

The program opens a TCP connection to `smtp.gmail.com` on port `587`, performs the SMTP greeting sequence, then upgrades the session to TLS with `STARTTLS`. After the TLS handshake, it authenticates using SMTP `AUTH LOGIN` and sends a basic email message.

The code is intentionally simple and is useful as a teaching example for how SMTP clients work at a low level.

## Project Structure

- `client.c` — SMTP client implementation
- `Makefile` — build rules for compiling the program

## Requirements

Before building, install the following:

- GCC or Clang
- OpenSSL development libraries
- A Gmail account with an app password enabled

On Debian/Ubuntu-based systems, the usual dependency install is:

```bash
sudo apt update
sudo apt install build-essential libssl-dev
```

## Build

From the project directory, run:

```bash
make
```

This produces the executable:

```bash
./client
```

## How It Works

The program performs the following sequence:

1. Creates a TCP socket to the SMTP server.
2. Connects to Gmail over port `587`.
3. Reads the initial SMTP greeting from the server.
4. Sends `EHLO` and `STARTTLS` commands.
5. Establishes a TLS connection with OpenSSL.
6. Repeats the `EHLO` sequence over the encrypted channel.
7. Sends `AUTH LOGIN` and then the base64-encoded email and password.
8. Sends the email headers and body using the SMTP `DATA` command.
9. Closes the session with `QUIT`.

## Configuration

Before running the client, update the values in `client.c`:

- `hostname` (currently `smtp.gmail.com`)
- `from_email`
- `to_email`
- SMTP login payload (base64-encoded username and password)

Important: Gmail requires an application password rather than the normal account password for many non-browser SMTP clients.

The relevant section in the code looks like this:

```c
char *from_email = "your_sender@gmail.com";
char *to_email = "recipient@gmail.com";
```

And the login step is handled here:

```c
send_smtp_ssl_cmd(ssl, "AUTH LOGIN\r\n", buffer);
send_smtp_ssl_cmd(ssl, base64_email, buffer);
send_smtp_ssl_cmd(ssl, "YOUR_BASE64_APP_PASSWORD\r\n", buffer);
```


## Example Usage

```bash
make
./client
```

The program prints the SMTP conversation between the client and the server so you can see the exact command exchange.


