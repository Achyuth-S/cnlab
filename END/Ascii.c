// ASCII +4 Encryption Client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORTNO 9999

int main(void) {
    char buf[256];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    printf("Socket created...\n");

    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    int result = connect(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    if (result == -1) {
        printf("Connection error...\n");
        close(sockfd);
        exit(1);
    }

    printf("Data to send to server: ");
    scanf("%s", buf);

    for (int i = 0; i < 256; i++) {
        if (buf[i] == '\0')
            break;
        else
            buf[i] = buf[i] + 4;
    }

    printf("Sending encrypted data to server...\n");
    write(sockfd, buf, sizeof(buf));
    printf("Closing socket and exiting...\n");

    close(sockfd);
    return 0;
}

// ASCII -4 Decryption Server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    char buf[256];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    printf("Socket created...\n");

    struct sockaddr_in seraddr, cliaddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    if (bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) == -1) {
        printf("Bind failed...\n");
        close(sockfd);
        exit(1);
    }
    printf("Socket binded to server address...\n");

    listen(sockfd, 5);
    printf("Server listening...\n");

    int clilen = sizeof(cliaddr);
    int newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
    if (newsockfd == -1) {
        printf("Connection acceptance failed...\n");
        close(sockfd);
        exit(1);
    }
    printf("Connection accepted!\n");

    read(newsockfd, buf, sizeof(buf));
    printf("Encrypted message received from client: %s\n", buf);

    for (int i = 0; i < 256; i++) {
        if (buf[i] == '\0')
            break;
        else
            buf[i] = buf[i] - 4;
    }

    printf("Decrypted message: %s\n", buf);
    printf("Closing socket and exiting...\n");

    close(newsockfd);
    close(sockfd);
    return 0;
}