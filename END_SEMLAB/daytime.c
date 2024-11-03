TCP DAYTIME- Server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#define PORTNO 9999
int main(void) {
char buf[256];
int buf2[256];
int sockfd = socket(AF
INET , SOCK
_
_
printf("Socket created...\n");
STREAM, 0);
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
printf("Server address binded to port...\n");
listen(sockfd, 5);
while(1) {
int newsockfd = accept(sockfd, (struct
sockaddr*)&cliaddr, &clilen);
int pid = fork();
if(pid == 0) {
time
t t;
_
time(&t);
printf("Current time: %s\n"
, ctime(&t));
strcpy(buf, ctime(&t));
printf("Sending to client...\n");
write(newsockfd, buf, sizeof(buf));
buf2[0] = getpid();
write(newsockfd, buf2, sizeof(buf2));
close(newsockfd);
exit(0);
} else {
close(newsockfd);
}
}
return 0;
}

TCP Daytime
//client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORTNO 9999
int main(void) {
char buf[256];
int buf2[256];
int sockfd = socket(AF
INET , SOCK
_
_
printf("Socket created!\n");
STREAM, 0);
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
printf("Waiting for server to provide time...\n");
read(sockfd, buf, sizeof(buf));
printf("Current date-time: %s\n"
, buf);
read(sockfd, buf2, sizeof(buf2));
printf("PID = %d\n"
, buf2[0]);
close(sockfd);
return 0;
}