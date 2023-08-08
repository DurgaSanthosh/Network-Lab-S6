#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_BUFFER 1024

int main() {
    int sockfd, newsockfd, clients[MAX_CLIENTS], client_count = 0;
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

    fd_set active_fds, read_fds;
    FD_ZERO(&active_fds);
    FD_SET(sockfd, &active_fds);

    printf("Server is listening...\n");

    while (1) {
	 read_fds = active_fds;
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
		    perror("select");
		    exit(EXIT_FAILURE);
		}

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == sockfd) {
                    len = sizeof(cliaddr);
                    newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
                    if (newsockfd < 0) {
                        perror("server accept failed");
                        exit(EXIT_FAILURE);
                    }

                    FD_SET(newsockfd, &active_fds);
                    clients[client_count++] = newsockfd;

                    printf("Client %d connected\n", newsockfd);
                } else {
                    char buffer[MAX_BUFFER];
                    int n = read(i, buffer, sizeof(buffer));
                    if (n <= 0) {
                        printf("Client %d disconnected\n", i);
                        close(i);
                        FD_CLR(i, &active_fds);
                    } else {
                        buffer[n] = '\0';
                        printf("Received from client %d: %s", i, buffer);
                        // Broadcast the message to other clients
                        for (int j = 0; j < client_count; ++j) {
                            if (clients[j] != i) {
                                write(clients[j], buffer, n); // Send the whole buffer
                            }
                        }
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}



