#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define PORT 8080
#define BUFF_SIZE 4096

int main()
{
    int proxy_socket = 0;
    int client_socket = 0;
    int server_socket = 0;

    struct sockaddr_in proxy_address;

    proxy_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (proxy_socket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    proxy_address.sin_family = AF_INET;
    proxy_address.sin_port = htons(PORT);
    proxy_address.sin_addr.s_addr = INADDR_ANY;

    int proxy_size = sizeof(proxy_address);

    if (bind(proxy_socket, (struct sockaddr *)&proxy_address, proxy_size) < 0)
    {
        perror("socket binding failed");
        close(proxy_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(proxy_socket, 3) < 0)
    {
        perror("listing failed");
        close(proxy_socket);
        exit(EXIT_FAILURE);
    }

    client_socket = accept(proxy_socket, NULL, NULL);

    char buffer[BUFF_SIZE];

    int bytes_read_client = recv(client_socket, buffer, BUFF_SIZE, 0);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    

    send(server_socket, buffer, bytes_read_client, 0);

    int bytes_read_server = recv(server_socket, buffer, BUFF_SIZE, 0);

    send(client_socket, buffer, bytes_read_server, 0);

    close(client_socket);
    close(server_socket);

    return 0;
}