}
UDP MATRIX ROWS-CLIENT
#define PORTNO 9999
int main(void)
{
int buf[256];
int sockfd = socket(AF
INET , SOCK
_
_
DGRAM, 0);
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
int m, n;
printf("Enter dimensions of matrix: ");
scanf("%d %d"
, &m, &n);
buf[0] = m;
buf[1] = n;
sendto(sockfd, buf, sizeof(buf), 0, (struct
sockaddr*)&seraddr, sizeof(seraddr));
for(int i = 0; i < m; i++)
{
printf("Enter data for Row#%d: "
, i + 1);
for(int j = 0; j < n; j++)
{
scanf("%d"
, &buf[j]);
}
sendto(sockfd, buf, sizeof(buf), 0, (struct
sockaddr*)&seraddr, sizeof(seraddr));
}
printf("Waiting for server matrix...\n");
int serlen = sizeof(seraddr);
recvfrom(sockfd, buf, sizeof(buf), 0, (struct
sockaddr*)&seraddr, &serlen);
printf("MATRIX:\n");
for(int i = 0; i < m; i++)
{
for(int j = 0; j < n; j++)
{
printf("%d\t"
, buf[i * n + j]);
}
printf("\n");
}
close(sockfd);
return 0;

UDP matrix rows-SERVER
#define PORTNO 9999
int main(void)
{
int buf[256];
int matrix[256][256];
int sockfd = socket(AF
INET , SOCK
_
_
DGRAM, 0);
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
printf("Socket binded!\n");
int m, n;
recvfrom(sockfd, buf, sizeof(buf), 0, (struct
sockaddr*)&cliaddr, &clilen);
m = buf[0];
n = buf[1];
printf("Dimensions received: %d x %d\n"
, m, n);
for(int i = 0; i < m; i++)
{
recvfrom(sockfd, buf, sizeof(buf), 0, (struct
sockaddr*)&cliaddr, &clilen);
printf("Received row#%d: "
, i + 1);
for(int j = 0; j < n; j++)
{
printf("%d\t"
, buf[j]);
matrix[i][j] = buf[j];
}
printf("\n");
}
printf("Sending matrix to client...\n");
for(int i = 0; i < m; i++)
{
for(int j = 0; j < n; j++)
{
buf[i * n + j] = matrix[i][j];
}
}
sendto(sockfd, buf, sizeof(buf), 0, (struct
sockaddr*)&cliaddr, sizeof(cliaddr));
close(sockfd);
return 0;
}