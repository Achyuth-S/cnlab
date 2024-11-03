// TCP Concurrent Math Server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORTNO 9999

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
            close(sockfd); // Close the listening socket in the child process
            printf("Connection accepted!\n");

            int a, b, res;
            char op;
            read(newsockfd, buf, sizeof(buf));

            a = buf[0];
            b = buf[1];
            op = (char)buf[2];
            printf("Received arithmetic expression: %d %c %d\n", a, op, b);

            switch (op) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/': res = (b != 0) ? a / b : 0; break; // Check for division by zero
                default: res = 0; // Default case for invalid operator
            }

            printf("Result = %d\n", res);
            printf("Sending result to client...\n");
            buf[0] = res;
            write(newsockfd, buf, sizeof(buf));

            close(newsockfd);
            exit(0);
        } else {
            close(newsockfd);  // Parent closes the new socket
        }
    }

    close(sockfd);
    return 0;
}

// TCP Concurrent Math Client
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

    int a, b;
    char op;
    printf("Operand 1: ");
    scanf("%d", &a);
    printf("Operand 2: ");
    scanf("%d", &b);
    printf("Operator (+, -, *, /): ");
    scanf(" %c", &op);

    buf[0] = a;
    buf[1] = b;
    buf[2] = op;

    printf("Sending expression %d %c %d to server...\n", a, op, b);
    write(sockfd, buf, sizeof(buf));

    printf("Waiting for response from server...\n");
    read(sockfd, buf, sizeof(buf));

    int res = buf[0];
    printf("Result = %d\n", res);

    close(sockfd);
    return 0;
}