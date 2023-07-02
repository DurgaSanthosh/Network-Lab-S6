#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

// Function to simulate data transmission
bool transmitData(int sequenceNumber) {
    // Simulating transmission success with a 70% probability
    if (rand() % 10 < 7)
        return true;
    return false;
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding");
        exit(1);
    }
    
    // Listen for incoming connections
    if (listen(sockfd, 1) < 0) {
        perror("Error listening");
        exit(1);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    clilen = sizeof(cli_addr);
    
    // Accept incoming connection
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error accepting connection");
        exit(1);
    }
    
    printf("Connection accepted.\n");
    
    int expectedSequenceNumber = 0;
    
    while (true) {
        // Receiving data
        int receivedSequenceNumber;
        ssize_t recvLen = recv(newsockfd, &receivedSequenceNumber, sizeof(receivedSequenceNumber), 0);
        if (recvLen <= 0) {
            perror("Error receiving data");
            exit(1);
        }
        
        if (receivedSequenceNumber == expectedSequenceNumber) {
            // Data received with the expected sequence number
            printf("Data received with sequence number: %d\n", receivedSequenceNumber);
            
            // Sending ACK
            printf("Sending ACK %d\n\n", receivedSequenceNumber);
            ssize_t sentLen = send(newsockfd, &receivedSequenceNumber, sizeof(receivedSequenceNumber), 0);
            if (sentLen <= 0) {
                perror("Error sending ACK");
                exit(1);
            }
            
            expectedSequenceNumber = (expectedSequenceNumber + 1) % 2;
        } else {
            // Data received with an incorrect sequence number, ignore and discard
            printf("Incorrect sequence number, discarding data...\n\n");
        }
    }
    
    close(newsockfd);
    close(sockfd);
    
    return 0;
}

