// Remove Duplicates Until Stop - Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    char buf[256];
    
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    // Send connection request to server
    int result = connect(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    if (result == -1) {
        printf("Connection error...\n");
        close(sockfd);
        exit(1);
    }

    // Until stop
    while (1) {
        // Get user sentence to send
        printf("Enter sentence to send to server: ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;  // Remove trailing newline

        printf("Sending sentence to server...\n");
        write(sockfd, buf, sizeof(buf));

        if (strcmp(buf, "Stop") == 0) {
            printf("Closing and stopping...\n");
            close(sockfd);
            break;
        }

        printf("Waiting for server response...\n");
        read(sockfd, buf, sizeof(buf));
        printf("Response from server: %s\n", buf);
    }

    return 0;
}

// Remove Duplicates Until Stop - Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main(void) {
    char buf[256];
    char words[256][256];
    char unique_words[256][256];

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
    printf("Server address bound to socket...\n");

    // Listen for connections
    listen(sockfd, 5);
    printf("Server listening...\n");

    // Accept connection
    int newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
    if (newsockfd == -1) {
        printf("Connection acceptance failed...\n");
        close(sockfd);
        exit(1);
    }
    printf("Connection accepted...\n");

    while (1) {
        read(newsockfd, buf, sizeof(buf));
        printf("Message sent by client: %s\n", buf);

        // Check for stop message
        if (strcmp(buf, "Stop") == 0) {
            printf("Closing and exiting...\n");
            close(newsockfd);
            close(sockfd);
            break;
        }

        // Tokenize the sentence into words
        int curr_word = 0;
        char* token = strtok(buf, " ");
        while (token != NULL) {
            strcpy(words[curr_word++], token);
            token = strtok(NULL, " ");
        }

        // Remove duplicates
        int unique_count = 0;
        for (int i = 0; i < curr_word; i++) {
            int unique = 1;
            for (int j = 0; j < unique_count; j++) {
                if (strcmp(unique_words[j], words[i]) == 0) {
                    unique = 0;
                    break;
                }
            }
            if (unique) {
                strcpy(unique_words[unique_count++], words[i]);
            }
        }

        // Construct response message
        memset(buf, 0, sizeof(buf));
        for (int i = 0; i < unique_count; i++) {
            strcat(buf, unique_words[i]);
            if (i < unique_count - 1) {
                strcat(buf, " ");
            }
        }

        printf("Sending message to client...\n");
        write(newsockfd, buf, sizeof(buf));
    }

    return 0;
}