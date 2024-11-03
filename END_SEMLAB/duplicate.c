Remove duplicates until stop
//client
//same headers as prev
#define PORTNO 9999
int main(void)
{
char buf[256];
//Create socket:
int sockfd = socket(AF
INET , SOCK
_
_
printf("Socket created...\n");
STREAM, 0);
//Set server address:
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
//Send connection request to server:
int result = connect(sockfd, (struct
sockaddr*)&seraddr, sizeof(seraddr));
if(result==-1)
{
printf("Connection error...\n");
close(sockfd);
exit(1);
}
//Until stop:
while(1)
{
//Get user sentence to send:
printf("Enter sentence to send to server: ");
gets(buf);
puts(buf);
printf("Sending sentence to server...\n");
write(sockfd, buf, sizeof(buf));
if(strcmp(buf,
"Stop")==0)
{
printf("Closing and stopping...\n");
close(sockfd);
exit(0);
}
printf("Waiting for server response...\n");
read(sockfd, buf, sizeof(buf));
printf("Response from server: %s\n"
, buf);
}
return 0;
}

REMOVE DUPLICATES UNTIL STOP - SERVER
#define PORTNO 9999
int main(void) {
char buf[256];
char words[256][256];
char unique
_
words[256][256];
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
int newsockfd = accept(sockfd, (struct
sockaddr*)&cliaddr, &clilen);
printf("Connection accepted...\n");
while(1) {
read(newsockfd, buf, sizeof(buf));
printf("Message sent by client: %s\n"
, buf);
if(strcmp(buf,
"Stop") == 0) {
printf("Closing and exiting...\n");
close(newsockfd);
close(sockfd);
exit(0);
}
int curr
word = 0;
_
char* token = strtok(buf,
" ");
while(token != NULL) {
strcpy(words[curr
_
word], token);
curr
word++;
_
token = strtok(NULL,
" ");
}
int total
count = curr
_
word;
_
curr
word = 0;
_
int unique
count = 0;
_
for(int i = 0; i < total
_
count; i++) {
int unique = 1;
for(int j = 0; j < unique
_
count; j++) {
if(strcmp(unique
_
words[j], words[i]) == 0) {
unique = 0;
break;
} }
if(unique) {
strcpy(unique
_
words[curr
_
word], words[i]);
curr
word++;
_
unique
count++;
_
} }
memset(buf, 0, sizeof(buf));
for(int i = 0; i < unique
_
count; i++) {
strcat(buf, unique
_
words[i]);
if(i < unique
_
count - 1) {
strcat(buf,
" ");
} }
printf("Sending message to client...\n");
write(newsockfd, buf, sizeof(buf));
}
return 0;}