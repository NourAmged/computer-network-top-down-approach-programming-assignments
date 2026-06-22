#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

struct client_message
{
    int number;
    char message[256];
};

int main()
{
    int sock = 0;
    int number;
    struct sockaddr_in server_address;

    struct client_message client;
    struct client_message server;

    ssize_t valread;

    char *message = "Client of Nour \n";

    strncpy(client.message, message, strlen(message));

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    char *ip = "127.0.0.1";

    // converting ip address from text to binary format

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) < 0)
    {
        perror("Invalid address / Address not supported\n");
        exit(EXIT_FAILURE);
    }

    // concent
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server!\n");

    while (1)
    {
        printf("pick number between 1 and 100 (above 100 will send final request before ending) \n");
        scanf("%d", &number);

        client.number = number;

        send(sock, (void *)&client, sizeof(client), 0);

        printf("client message sent \n");

        valread = read(sock, (void *)&server, sizeof(server));
        printf("%s \n", server.message);
        printf("sum number is %d \n", server.number);
        if (number > 100)
        {
            exit(EXIT_SUCCESS);
        }
    }

    close(sock);

    return 0;
}