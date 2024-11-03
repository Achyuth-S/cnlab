// TCP Concurrent Sort + PID Server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORTNO 9999

void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    int buf[256];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    printf("Socket created!\n");

    struct sockaddr_in seraddr, cliaddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    if (bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) == -1) {
        printf("Bind failed...\n");
        close(sockfd);
        exit(1);
    }
    printf("Server address bound to socket...\n");

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
            close(sockfd); // Close listening socket in child process
            printf("Connection accepted!\n");

            int n;
            read(newsockfd, buf, sizeof(buf));
            n = buf[0];
            printf("Size of array: %d\n", n);

            read(newsockfd, buf, sizeof(buf));
            sort(buf, n);

            printf("Sorted array:\n");
            for (int i = 0; i < n; i++) {
                printf("%d\t", buf[i]);
            }
            printf("\n");

            buf[n] = getpid();
            printf("Sending sorted array and PID to client...\n");
            write(newsockfd, buf, sizeof(buf));

            printf("Closing and exiting...\n");
            close(newsockfd);
            exit(0);
        } else {
            close(newsockfd);  // Parent closes the new socket
        }
    }

    close(sockfd);
    return 0;
}

// TCP Concurrent Sort + PID Client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    int buf[256];
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

    printf("Enter number of elements: ");
    int n;
    scanf("%d", &n);
    buf[0] = n;
    write(sockfd, buf, sizeof(buf));

    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Element #%d: ", i + 1);
        scanf("%d", &buf[i]);
    }

    printf("Sending array to server...\n");
    write(sockfd, buf, sizeof(buf));

    printf("Waiting for sorted array and PID from server...\n");
    read(sockfd, buf, sizeof(buf));

    printf("SORTED ARRAY RECEIVED:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", buf[i]);
    }
    printf("\n");

    printf("PID of server process = %d\n", buf[n]);

    close(sockfd);
    return 0;
}