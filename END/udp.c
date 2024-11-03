// UDP Matrix Rows - Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    int buf[256];
    
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    printf("Socket created...\n");

    // Set server address
    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    int m, n;
    printf("Enter dimensions of matrix (rows and columns): ");
    scanf("%d %d", &m, &n);

    // Send matrix dimensions
    buf[0] = m;
    buf[1] = n;
    sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&seraddr, sizeof(seraddr));

    // Send matrix rows
    for (int i = 0; i < m; i++) {
        printf("Enter data for Row #%d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf("%d", &buf[j]);
        }
        sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&seraddr, sizeof(seraddr));
    }

    // Receive and print the matrix from server
    printf("Waiting for server matrix...\n");
    int serlen = sizeof(seraddr);
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&seraddr, &serlen);

    printf("MATRIX:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", buf[i * n + j]);
        }
        printf("\n");
    }

    close(sockfd);
    return 0;
}

// UDP Matrix Rows - Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    int buf[256];
    int matrix[256][256];

    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    printf("Socket created...\n");

    // Set server address
    struct sockaddr_in seraddr, cliaddr;
    int clilen = sizeof(cliaddr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) == -1) {
        printf("Binding failed...\n");
        close(sockfd);
        exit(1);
    }
    printf("Socket binded!\n");

    // Receive matrix dimensions
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &clilen);
    int m = buf[0];
    int n = buf[1];
    printf("Dimensions received: %d x %d\n", m, n);

    // Receive matrix rows
    for (int i = 0; i < m; i++) {
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &clilen);
        printf("Received row #%d: ", i + 1);
        for (int j = 0; j < n; j++) {
            printf("%d\t", buf[j]);
            matrix[i][j] = buf[j];
        }
        printf("\n");
    }

    // Send the complete matrix back to the client
    printf("Sending matrix to client...\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            buf[i * n + j] = matrix[i][j];
        }
    }
    sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));

    close(sockfd);
    return 0;
}