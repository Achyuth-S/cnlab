TCP CONCURRENT MATH - SERVER
#define PORTNO 9999
int main(void) {
int buf[256];
int sockfd = socket(AF
INET , SOCK
_
printf("Socket created!\n");
_
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
printf("Server address binded to socket...\n");
listen(sockfd, 5);
while(1) {
int newsockfd = accept(sockfd, (struct
sockaddr*)&cliaddr, &clilen);
int pid = fork();
if(pid == 0) {
printf("Connection accepted!\n");
int a, b;
char op;
read(newsockfd, buf, sizeof(buf));
a = buf[0];
b = buf[1];
op = buf[2];
printf("Received arithmetic expression: %d %c
%d\n"
, a, op, b);
int res;
switch(op) {
case '+': res = a + b; break;
case '
-
': res = a - b; break;
case '*': res = a * b; break;
case '/': res = a / b; break;
}
printf("Result = %d\n"
, res);
printf("Sending to client...\n");
buf[0] = res;
write(newsockfd, buf, sizeof(buf));
close(newsockfd);
exit(0);
} else {
close(newsockfd);
}
}
return 0;
}

TCP concurrent remote math server
client
//same headers
#define PORTNO 9999
int main(void) {
int buf[256];
int sockfd = socket(AF
INET , SOCK
_
printf("Socket created...\n");
_
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
int a, b;
char op;
printf("Op 1: ");
scanf("%d"
, &a);
printf("Op 2: ");
scanf("%d"
, &b);
printf("Operator: ");
scanf(" %c"
, &op);
buf[0] = a;
buf[1] = b;
buf[2] = op;
printf("Sending expression %d %c %d to server...\n"
,
a, op, b);
write(sockfd, buf, sizeof(buf));
int res;
printf("Waiting for response from server...\n");
read(sockfd, buf, sizeof(buf));
res = buf[0];
printf("Result = %d\n"
, res);
close(sockfd);
return 0;
}