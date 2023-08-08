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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("connection with the server failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Start chatting:\n");

    while (1) {
        char message[MAX_BUFFER];
        fgets(message, sizeof(message), stdin);

        write(sockfd, message, strlen(message));
        if (strcmp(message, "exit\n") == 0) {
            printf("Disconnected from the server.\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}

