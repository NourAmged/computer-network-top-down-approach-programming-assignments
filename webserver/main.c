#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8080
#define BUFFER_SIZE 1024

char *parse_route(const char *route)
{
    if (strcmp(route, "/") == 0)
        return "index.html";
    else if (strcmp(route, "/hello") == 0)
        return "hello.html";
    else
        return "error.html";
}

void send_html(int *sock, const char *file)
{
    FILE *html = fopen(file, "r");
    if (!html)
    {
        perror("could not open html file");
        fclose(html);
        return;
    }

    char buffer[BUFFER_SIZE] = {0};
    size_t read = 0;

    char *header = "HTTP/1.1 200 OK \r\nContent-Type: text/html\r\n\r\n";

    if (strcmp(file, "error.html") == 0)
        header = "HTTP/1.1 404 Not Found \r\nContent-Type: text/html\r\n\r\n";

    send(*sock, header, strlen(header), 0);

    while ((read = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, html)) > 0)
    {
        send(*sock, buffer, read, 0);
    }
    fclose(html);
}

int main()
{

    int serverfd;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverfd < 0)
    {
        perror("could not initiate server file descriptor");
        close(serverfd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(serverfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("could not bind socket server");
        close(serverfd);
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, 3) < 0)
    {
        perror("could not listen on server socket");
        close(serverfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    struct sockaddr_in client_address;

    socklen_t client_len = sizeof(client_address);

    int *client_socket = malloc(sizeof(int));

    while (1)
    {
        int *client_socket = malloc(sizeof(int));

        *client_socket = accept(serverfd, (struct sockaddr *)&client_address, &client_len);
        if (*client_socket < 0)
        {
            perror("could not accept client");
            continue;
        }

        printf("Client connected \n");

        char client_buffer[BUFFER_SIZE] = {0};
        recv(*client_socket, client_buffer, BUFFER_SIZE, 0);

        // works with GET

        char *token = client_buffer + 4;
        char *route = strtok(token, " ");

        send_html(client_socket, parse_route(route));

        close(*client_socket);

        printf("Client disconnected \n");

        free(client_socket);
    }

    close(serverfd);

    return 0;
}