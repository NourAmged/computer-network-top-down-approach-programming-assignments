#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>

#define PORT 587
#define BUFF_SIZE 4096

void send_smtp_cmd(int sock_fd, const char *cmd, char *buffer)
{
    send(sock_fd, cmd, strlen(cmd), 0);
    printf("Client: \n%s\n", cmd);

    memset(buffer, 0, BUFF_SIZE);

    int bytes = recv(sock_fd, buffer, BUFF_SIZE - 1, 0);

    if (bytes < 0)
    {
        perror("recv failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';

    printf("Server: \n%s\n", buffer);
}

void send_smtp_ssl_cmd(SSL *ssl, const char *cmd, char *buffer)
{
    SSL_write(ssl, cmd, strlen(cmd));

    printf("Client: \n%s\n", cmd);

    memset(buffer, 0, BUFF_SIZE);

    int bytes = SSL_read(ssl, buffer, BUFF_SIZE - 1);

    if (bytes < 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';

    printf("Server: \n%s\n", buffer);
}

int main()
{
    int sock_fd = 0;

    SSL_CTX *ctx;
    SSL *ssl;
    struct sockaddr_in mail_server = {0};

    // init openssl
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
    {
        perror("Warning: Could not load default verification paths.\n");
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[BUFF_SIZE];

    if (sock_fd < 0)
    {
        perror("error creating socket");
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    mail_server.sin_family = AF_INET;
    mail_server.sin_port = htons(PORT);

    char *hostname = "smtp.gmail.com";

    struct addrinfo hint = {0};
    struct addrinfo *results;

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname, NULL, &hint, &results);

    if (status < 0)
    {
        perror("error in converting hostname to ip");
        close(sock_fd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    struct in_addr server_ip = ((struct sockaddr_in *)results->ai_addr)->sin_addr;
    mail_server.sin_addr = server_ip;

    if (connect(sock_fd, (struct sockaddr *)&mail_server, sizeof(mail_server)) < 0)
    {
        perror("connection failed");
        close(sock_fd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(results);

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock_fd);

    int bytes = recv(sock_fd, buffer, BUFF_SIZE - 1, 0);

    if (bytes < 0)
    {
        perror("recv failed");
        close(sock_fd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';

    printf("Mail server response:\n%s\n", buffer);

    // SMTP handshake & mail data sequence
    send_smtp_cmd(sock_fd, "EHLO Nour\r\n", buffer);
    send_smtp_cmd(sock_fd, "STARTTLS\r\n", buffer);

    SSL_set1_host(ssl, hostname);
    SSL_set_verify(ssl, SSL_VERIFY_PEER, NULL);

    if (SSL_connect(ssl) != 1)
    {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    send_smtp_ssl_cmd(ssl, "EHLO Nour\r\n", buffer);
    // loginin user
    send_smtp_ssl_cmd(ssl, "AUTH LOGIN\r\n", buffer);
    // gmail
    send_smtp_ssl_cmd(ssl, "base64_gmail\r\n", buffer);
    // application password
    send_smtp_ssl_cmd(ssl, "base64_application_password\r\n", buffer);

    const *from_email = "example@gmail.com";
    const *to_email = "example2@gmail.com";

    send_smtp_ssl_cmd(ssl, "MAIL FROM:<example@gmail.com>\r\n", buffer);
    send_smtp_ssl_cmd(ssl, "RCPT TO:<example2@gmail.com>\r\n", buffer);
    send_smtp_ssl_cmd(ssl, "DATA\r\n", buffer);

    // email message
    snprintf(buffer, BUFF_SIZE,
             "Subject: Test smtp in c\r\n"
             "From: %s\r\n"
             "To: %s\r\n"
             "\r\n"
             "This message has been sent using smtp in c"
             ".\r\n",
             from_email, to_email);

    send_smtp_ssl_cmd(ssl, buffer, buffer);

    send_smtp_ssl_cmd(ssl, "QUIT\r\n", buffer);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sock_fd);
    return 0;
}