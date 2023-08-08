#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    int n, len;

    while (1) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);// recvfrom for udp and recieve for tcp to recieve
        buffer[n] = '\0';
        printf("Received: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {//checks whether the first 4 characters of the buffer string are exactly the same as the string "exit".
            printf("Server Exit...\n");
            break;
        }

        printf("Enter a message to send to client: ");
        fgets(buffer, sizeof(buffer), stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}

