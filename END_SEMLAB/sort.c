TCP Concurrent sort + pid
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORTNO 9999
int main(void) {
int buf[256];
int sockfd = socket(AF
INET , SOCK
_
_
STREAM, 0);
printf("Socket created...\n");
struct sockaddr
in seraddr;
_
seraddr.sin
_
family = AF
INET ;
_
seraddr.sin
addr.s
addr = inet
_
_
_
addr("127.0.0.1");
seraddr.sin
_port = htons(PORTNO);
int result = connect(sockfd, (struct
sockaddr*)&seraddr, sizeof(seraddr));
if(result == -1) {
printf("Connection error...\n");
close(sockfd);
exit(1);
}
printf("Enter number of elements: ");
int n;
scanf("%d"
, &n);
printf("Sending size to server...\n");
buf[0] = n;
write(sockfd, buf, sizeof(buf));
for(int i = 0; i < n; i++) {
printf("Entry #%d: "
, i + 1);
scanf("%d"
, &buf[i]);
}
printf("Sending array to server...\n");
write(sockfd, buf, sizeof(buf));
printf("Waiting for server to return sorted array...\n");
read(sockfd, buf, sizeof(buf));
printf("SORTED ARRAY RECEIVED:\n");
for(int i = 0; i < n; i++) {
printf("%d\t"
, buf[i]);
}
printf("\n");
printf("PID = %d\n"
, buf[n]);
close(sockfd);
return 0;
}

TCP CONCURRENT SORT+PID
Server
#define PORTNO 9999
void sort(int arr[], int n) {
for(int i = 0; i < n - 1; i++) {
for(int j = 0; j < n - 1 - i; j++) {
if(arr[j] > arr[j + 1]) {
int temp = arr[j];
arr[j] = arr[j + 1];
arr[j + 1] = temp;
} } } }
int main(void) {
int buf[256];
int sockfd = socket(AF
INET , SOCK
_
_
STREAM, 0);
printf("Socket created...\n");
struct sockaddr
in seraddr, cliaddr;
_
int clilen = sizeof(cliaddr);
seraddr.sin
_
family = AF
INET ;
_
seraddr.sin
addr.s
addr = inet
_
_
_
addr("127.0.0.1");
seraddr.sin
_port = htons(PORTNO);
bind(sockfd, (struct sockaddr*)&seraddr,
sizeof(seraddr));
printf("Server address binded to socket...\n");
listen(sockfd, 5);
while(1) {
int newsockfd = accept(sockfd, (struct
sockaddr*)&cliaddr, &clilen);
int pid = fork();
if(pid == 0) {
int n;
printf("Connection accepted!\n");
read(newsockfd, buf, sizeof(buf));
n = buf[0];
printf("Size of array: %d\n"
, n);
read(newsockfd, buf, sizeof(buf));
sort(buf, n);
printf("Sorted array:\n");
for(int i = 0; i < n; i++) {
printf("%d\t"
, buf[i]);
}
printf("\n");
buf[n] = getpid();
printf("Sending to client...\n");
write(newsockfd, buf, sizeof(buf));
printf("Closing and exiting...\n");
close(newsockfd);
exit(0);
} else {
close(newsockfd);
}
}
return 0;
}