#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define PORT 8080

int main()
{
    int proxy_socket = 0;
    int client_socket = 0;

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

    return 0;
}