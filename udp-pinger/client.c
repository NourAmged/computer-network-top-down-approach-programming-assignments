#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12000
#define BUFF_SIZE 256

int main()
{

    char buffer[256];
    int client_fd;

    struct sockaddr_in server_address;

    char *message = "hello from client\n";

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0)
    {
        perror("failed creating socket\n");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    char *ip = "127.0.0.1";

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) < 0)
    {
        perror("Invalid address\n");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection Failed \n");
    }

    ssize_t size_server = sizeof(server_address);

    sendto(client_fd, message, sizeof(message), 0, (struct sockaddr *)&server_address, &size_server);

    ssize_t val_read = recvfrom(client_fd, buffer, sizeof(message), 0, (struct sockaddr *)&server_address, &server_address);

    return 0;
}
