#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>

const int MAXLEN = 1024;
void str_echo(int sockfd)
{
  char buf[MAXLEN];
  int n;
  while ( (n = read(sockfd, buf, MAXLEN)) > 0)
  {
    if (send(sockfd, buf, n, 0) < 0)
    {
      perror("str_echo: send error");
    }
  }
  if (n < 0)
  {
    perror("str_echo: read error");
  }
}
int main(int argc, char **argv)
{
  int serverSock, clientSock;
  struct sockaddr_in serverAddr, clientAddr;
  serverSock = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(5011);
 
  if (bind(serverSock,(struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("bind error");
    exit(1);
  }
  
  if (listen(serverSock, 0) < 0)
  {
    perror("listen error");
    exit(0);
  } 

  while(1)
  {
    int cliLen = sizeof(clientAddr);
    clientSock = accept(serverSock,
                        (struct sockaddr*)&clientAddr, 
                        (socklen_t *)&cliLen);
    str_echo(clientSock);
  }

  return 0;
}

