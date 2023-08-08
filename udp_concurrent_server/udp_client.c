#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define MAX_BUFFER 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];

    // Send a request to the server
    sendto(sockfd, "Time?", strlen("Time?"), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    // Receive and print the time from the server
    int n = recvfrom(sockfd, (char*)buffer, MAX_BUFFER, MSG_WAITALL, NULL, NULL);
    buffer[n] = '\0';
    printf("Received from server: %s\n", buffer);

    close(sockfd);

    return 0;
}

