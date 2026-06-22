#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>

#define PORT 8080

struct server_message
{
    int number;
    char message[256];
};

int main()
{

    int server_fd, new_socket;

    ssize_t valread;

    char *message = "Hello form server \n";

    struct sockaddr_in address;
    struct server_message server;
    struct server_message client;

    strncpy(server.message, message, strlen(message));

    int addresslen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // error handling
    if (server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // binding
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, addresslen) < 0)
    {
        perror("binding failed");
        exit(EXIT_FAILURE);
    }

    // listen

    if (listen(server_fd, 3) < 0)
    {
        perror("listing failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept coming request

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addresslen);
    if (new_socket < 0)
    {
        perror("error in accepting coming request");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted! \n");

    while (1)
    {
        srand(time(NULL)); // seed with current time

        int random_number = 1 + rand() % 100; // generate random number between 1 and 100;

        valread = read(new_socket, (void *)&client, sizeof(server));

        if (valread < 0)
        {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }

        printf("%s \n", client.message);

        server.number = random_number + client.number;

        printf("Client number %d \n", client.number);
        printf("Server number %d \n", random_number);

        send(new_socket, (void *)&server, sizeof(server), 0);
        printf("server message sent\n");

        if (client.number > 100)
        {
            exit(EXIT_SUCCESS);
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}