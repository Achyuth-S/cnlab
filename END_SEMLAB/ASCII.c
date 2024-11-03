ASCII +4 encryption & decryption
//Client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#define PORTNO 9999
int main(void)
{
char buf[256];:
int sockfd = socket(AF
INET , SOCK
_
_
printf("Socket created...\n");
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
:
int result = connect(sockfd, (struct
sockaddr*)&seraddr, sizeof(seraddr));
if(result==-1)
{
printf("Connection error...\n");
close(sockfd);
exit(1);
}
printf("Data to send to server: ");
scanf("%s"
, buf);
for(int i = 0; i < 256; i++)
{
if(buf[i] == 0)
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

ASCII 4+4 ENCRYPTION DECRYPTION
//SERVER
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
printf("Socket binded to server address...\n");
listen(sockfd, 5);
int newsockfd = accept(sockfd, (struct
sockaddr*)&cliaddr, &clilen);
printf("Connection accepted!\n");
read(newsockfd, buf, sizeof(buf));
printf("Encrypted message received from client: %s\n"
,
buf);
for(int i = 0; i < 256; i++) {
if(buf[i] == 0) {
break;
} else {
buf[i] = buf[i] - 4;
} }
printf("Decrypted message: %s\n"
, buf);
printf("Closing socket and exiting...\n");
close(newsockfd);
close(sockfd);
return 0;
}