#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define PORT 12000
#define BUFF_SIZE 256

int main()
{

    struct timeval timeout = {1, 0};

    char buffer[BUFF_SIZE];
    int client_fd;

    struct sockaddr_in server_address;

    char *message = "hello from client\n";

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_fd < 0)
    {
        perror("failed creating socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    char *ip = "127.0.0.1";

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) < 0)
    {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    socklen_t server_size = sizeof(server_address);

    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    for (int i = 0; i < 10; i++)
    {
        clock_t start_time = clock();

        sendto(client_fd, message, strlen(message), 0, (struct sockaddr *)&server_address, server_size);

        ssize_t val_read = recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, &server_size);

        if (val_read < 0)
        {
            perror("Timeout");
            printf("\n");
            continue;
        }

        clock_t end_time = clock();
        double rtt = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("%s \nRTT: %f \n \n", buffer, rtt);

    }

    return 0;
}
