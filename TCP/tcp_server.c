#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};
    char *response = "Hello from server!";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    	//AF_INET specifies the address family or domain of the socket. In this case,AF_INET indicates that the socket will use IPv4 addresses
    	//SOCK_STREAM specifies the type of the socket. SOCK_STREAM indicates a TCP socket, which provides a reliable, connection-oriented stream of data.
    	//0 or IPPROTO_TCP specifies the protocol to be used. In this case, 0 indicates that the default protocol for the given socket type (TCP) should be used.
    	
    	
    // Set up the server address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    	//htons() function is used to convert the port number from host byte order to network byte order.

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    	//3: The maximum number of pending connections that can be queued for acceptance.

    printf("Server listening on port %d\n", PORT);

    // Accept a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    	//accept() function blocks until an incoming client connection is received on the server socket.

    // Receive data from the client
    int valread = read(new_socket, buffer, MAX_BUFFER_SIZE);
    printf("Received message from client: %s\n", buffer);

    // Send a response to the client
    send(new_socket, response, strlen(response), 0);
    printf("Response sent to client\n");
	//0: Additional flags, in this case, set to 0 for default behavior.
    return 0;
}

