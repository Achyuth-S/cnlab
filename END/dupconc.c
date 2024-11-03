// TCP Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define PORTNO 9999
#define MAX_WORDS 256
#define MAX_WORD_LENGTH 256

void* handle_client(void* arg) {
    int newsockfd = *((int*)arg);
    free(arg); // Free the allocated memory for newsockfd
    char buffer[1024];
    
    // Read the sentence from the client
    read(newsockfd, buffer, sizeof(buffer));
    printf("Received from client: %s\n", buffer);

    // Process the sentence
    char* words[MAX_WORDS];
    char* unique_words[MAX_WORDS];
    int word_count = 0;
    int unique_count = 0;
    int occurrences[MAX_WORDS] = {0};

    // Tokenize the input sentence
    char* token = strtok(buffer, " ");
    while (token != NULL) {
        words[word_count++] = token;
        token = strtok(NULL, " ");
    }

    // Find unique words and count occurrences
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                found = 1;
                occurrences[j]++;
                break;
            }
        }
        if (!found) {
            unique_words[unique_count] = words[i];
            occurrences[unique_count] = 1; // First occurrence
            unique_count++;
        }
    }

    // Construct the resultant sentence
    char result[1024] = "";
    for (int i = 0; i < unique_count; i++) {
        strcat(result, unique_words[i]);
        strcat(result, " ");
    }

    // Send the resultant sentence back to the client
    write(newsockfd, result, strlen(result) + 1); // Include null terminator
    printf("Sent to client: %s\n", result);

    close(newsockfd);
    return NULL;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in servaddr, cliaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORTNO);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);
    printf("Server listening on port %d...\n", PORTNO);

    while (1) {
        socklen_t len = sizeof(cliaddr);
        int* newsockfd = malloc(sizeof(int));
        *newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
        if (*newsockfd < 0) {
            perror("Accept failed");
            free(newsockfd);
            continue;
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        // Create a new thread for the client
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, newsockfd);
        pthread_detach(tid); // Detach the thread to allow it to clean up after itself
    }

    close(sockfd);
    return 0;
}

// TCP Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORTNO 9999

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORTNO);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    printf("Enter a sentence: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

    // Send the sentence to the server
    write(sockfd, buffer, strlen(buffer) + 1); // Include null terminator

    // Receive the processed sentence from the server
    read(sockfd, buffer, sizeof(buffer));
    printf("Result from server: %s\n", buffer);

    close(sockfd);
    return 0;
}