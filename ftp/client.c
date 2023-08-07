#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    FILE *file;
    ssize_t bytes_read;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Get the file name from the user
    printf("Enter the file name: ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);

    // Send the file name to the server
    if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }

    // Receive the server PID
    memset(buffer, 0, MAX_BUFFER_SIZE);
    if (recv(client_socket, buffer, MAX_BUFFER_SIZE, 0) > 0) {
        printf("%s\n", buffer);
    } else {
        printf("Error receiving server PID.\n");
    }

    // Receive data from the server and display it
    while ((bytes_read = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    close(client_socket);

    return 0;
}

