#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
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

int main()
{
    int sock_fd = 0;
    struct sockaddr_in mail_server = {0};

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[BUFF_SIZE];

    if (sock_fd < 0)
    {
        perror("error creating socket");
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
        exit(EXIT_FAILURE);
    }

    struct in_addr server_ip = ((struct sockaddr_in *)results->ai_addr)->sin_addr;
    mail_server.sin_addr = server_ip;

    if (connect(sock_fd, (struct sockaddr *)&mail_server, sizeof(mail_server)) < 0)
    {
        perror("connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    int bytes = recv(sock_fd, buffer, BUFF_SIZE - 1, 0);

    if (bytes < 0)
    {
        perror("recv failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';

    printf("Mail server response:\n%s\n", buffer);

    // SMTP handshake & mail data sequence
    send_smtp_cmd(sock_fd, "EHLO Nour\r\n", buffer);
    send_smtp_cmd(sock_fd, "MAIL FROM:<test@gmail.com>\r\n", buffer);
    send_smtp_cmd(sock_fd, "RCPT TO:<test2@gmail.com>\r\n", buffer);
    send_smtp_cmd(sock_fd, "DATA\r\n", buffer);

    freeaddrinfo(results);
    close(sock_fd);
    return 0;
}