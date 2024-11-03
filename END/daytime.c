// TCP Daytime Server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#define PORTNO 9999

int main(void) {
    char buf[256];
    int buf2[256];
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
    printf("Server address bound to port...\n");

    listen(sockfd, 5);
    printf("Server listening...\n");

    while (1) {
        int clilen = sizeof(cliaddr);
        int newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (newsockfd == -1) {
            printf("Connection acceptance failed...\n");
            continue;
        }

        int pid = fork();
        if (pid == 0) {  // Child process
            time_t t;
            time(&t);
            printf("Current time: %s\n", ctime(&t));

            strcpy(buf, ctime(&t));
            printf("Sending to client...\n");
            write(newsockfd, buf, sizeof(buf));

            buf2[0] = getpid();
            write(newsockfd, buf2, sizeof(buf2));

            close(newsockfd);
            exit(0);
        } else {
            close(newsockfd);  // Parent closes the new socket
        }
    }

    close(sockfd);
    return 0;
}

// TCP Daytime Client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    char buf[256];
    int buf2[256];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    printf("Socket created!\n");

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

    printf("Waiting for server to provide time...\n");
    read(sockfd, buf, sizeof(buf));
    printf("Current date-time: %s\n", buf);

    read(sockfd, buf2, sizeof(buf2));
    printf("PID = %d\n", buf2[0]);

    close(sockfd);
    return 0;
}