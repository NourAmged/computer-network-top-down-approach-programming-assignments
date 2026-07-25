#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define PORT 12000
#define BUFF_SIZE 256

void to_uppercase(char *str)
{
    while (*str)
    {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main()
{
    int server_fd;

    struct sockaddr_in address, client_address;

    char message[BUFF_SIZE];

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int address_len = sizeof(address);

    if (bind(server_fd, (struct sockaddr *)&address, address_len) < 0)
    {
        perror("socket binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listing on port %d ... \n", PORT);

    srand(time(NULL));

    while (1)
    {

        int random_number = rand() % 10;

        socklen_t client_size = sizeof(client_address);

        ssize_t val_read = recvfrom(server_fd, &message, sizeof(message) - 1, 0, (struct sockaddr *)&client_address, &client_size);

        if (val_read < 0)
        {
            perror("error reading incoming request");
            continue;
        }

        message[val_read] = '\0';

        if (random_number < 4)
            continue;

        to_uppercase(message);

        if (sendto(server_fd, message, val_read, 0, (struct sockaddr *)&client_address, client_size) < 0)
        {
            perror("sending failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}