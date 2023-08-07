#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

// Server code for handling client connection
void handle_client(int client_socket) {
    char buffer[MAX_BUFFER_SIZE];// to store data during communication w client
    FILE *file;
    pid_t server_pid = getpid(); // Get the server's PID from unistd.h

    // Send the server's PID to the client
    sprintf(buffer, "Server PID: %d", server_pid);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive the file name from the client
    memset(buffer, 0, MAX_BUFFER_SIZE);
    int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    // Remove the newline character from the file name
    if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    printf("Client requested file: %s\n", buffer);

    // Try to open the requested file
    file = fopen(buffer, "rb");
    if (file == NULL) {
        // File not found, send an appropriate message to the client
        strcpy(buffer, "File not found.");
        send(client_socket, buffer, strlen(buffer), 0);
        printf("File not found: %s\n", buffer);
    } else {
        // File found, send the file to the client
        while ((bytes_received = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            if (send(client_socket, buffer, bytes_received, 0) < 0) {
                perror("Error sending data");
                break;
            }
        }
        fclose(file);
        printf("File sent: %s\n", buffer);
    }

    close(client_socket);
    printf("Connection with client closed.\n");
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    pid_t child_pid;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening for incoming connections...\n");

    while (1) {
        // Accept incoming connection
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");

        // Fork a new process to handle the client connection concurrently
        child_pid = fork();
        if (child_pid < 0) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Child process - handle client connection
            close(server_socket);
            handle_client(client_socket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process - close the client socket and continue accepting new connections
            close(client_socket);
            waitpid(-1, NULL, WNOHANG); // Clean up zombie processes
        }
    }

    close(server_socket);

    return 0;
}

