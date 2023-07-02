#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define TOTAL_PACKETS 10

// Function to simulate data transmission
bool transmitData(int sequenceNumber) {
    // Simulating transmission success with a 70% probability
    if (rand() % 10 < 7)
        return true;
    return false;
}

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Error converting server IP address");
        exit(1);
    }
    
    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }
    
    int sequenceNumber = 0;
    
    for (int i = 0; i < TOTAL_PACKETS; i++) {
        // Sending data
        printf("Sending data with sequence number: %d\n", sequenceNumber);
        ssize_t sentLen = send(sockfd, &sequenceNumber, sizeof(sequenceNumber), 0);
        if (sentLen <= 0) {
            perror("Error sending data");
            exit(1);
        }
        
        // Receiving ACK
        int receivedAck;
        ssize_t recvLen = recv(sockfd, &receivedAck, sizeof(receivedAck), 0);
        if (recvLen <= 0) {
            perror("Error receiving ACK");
            exit(1);
        }
        
        if (receivedAck == sequenceNumber) {
            // ACK received for the sent sequence number
            printf("ACK %d received\n\n", receivedAck);
            sequenceNumber = (sequenceNumber + 1) % 2;
        } else {
            // ACK received for an incorrect sequence number, resend the data
            printf("ACK %d received, resending data...\n\n", receivedAck);
        }
    }
    
    close(sockfd);
    
    return 0;
}

