#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

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

    printf("Time server is listening...\n");

    while (1) {
        char buffer[MAX_BUFFER];
        socklen_t len = sizeof(cliaddr);

        int n = recvfrom(sockfd, (char*)buffer, MAX_BUFFER, MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';

        // Get current time
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        snprintf(buffer, MAX_BUFFER, "Current time: %s", asctime(timeinfo));

        // Send time to client
        sendto(sockfd, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&cliaddr, len);
    }

    close(sockfd);

    return 0;
}

