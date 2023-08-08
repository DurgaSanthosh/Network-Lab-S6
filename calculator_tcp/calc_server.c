#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <math.h>

#define PORT 8080
#define MAX_BUFFER 1024

double calculate(double a, double b, char operator) {
    switch (operator) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        case '^':
            return pow(a, b);
        default:
            return 0.0; // Invalid operator
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    // Listen for incoming connections
    if (listen(sockfd, 5) != 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept and handle incoming connections
    len = sizeof(cliaddr);
    connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if (connfd < 0) {
        perror("server accept failed");
        exit(EXIT_FAILURE);
    }

    char expression[MAX_BUFFER];
    char result[MAX_BUFFER];

    while (1) {
        bzero(expression, sizeof(expression));
        read(connfd, expression, sizeof(expression));

        double a, b;
        char operator;

        sscanf(expression, "%lf %c %lf", &a, &operator, &b);
        double res = calculate(a, b, operator);
        sprintf(result, "Result: %.2lf\n", res);
        write(connfd, result, strlen(result));
    }

    close(connfd);
    close(sockfd);

    return 0;
}

