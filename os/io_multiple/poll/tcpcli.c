#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <unistd.h>
#include <poll.h>

int max ( int a, int b ) { return a > b ? a : b; }
const int MAXLINE = 1024;
void str_cli(FILE *fp, int sockfd)
{
  struct pollfd fds[2];
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;
  fds[1].fd = fileno(fp);
  fds[1].events = POLLIN;
  int timeoutMs = 10000;
  char sendline[MAXLINE], recvline[MAXLINE];
  int len = sizeof(fds) / sizeof(fds[0]);
  for ( ; ; ) {
    bzero(sendline, MAXLINE);
    bzero(recvline, MAXLINE);

    poll(fds, len, timeoutMs);
    // socket 可读
    if (fds[0].revents == POLLIN)
    {
	  if (read(sockfd, recvline, MAXLINE) < 0)
      {
        perror("str_cli: server terminated prematurely");
	  } else {
        fputs(recvline, stdout);
      }
    }
    // 终端数据可读
    if (fds[1].revents == POLLIN)
    {
	  if (fgets(sendline, MAXLINE, fp) == NULL) return;		/* all done */
	  if (write(sockfd, sendline, strlen(sendline)) < 0)
      {
        perror("str_cli: write error");
      }
    }
  }
}

int main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2)
  {
    printf("usage: tcpcli <IPaddress>");
    exit(1);
  }


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("sock create error");
    exit(1);
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(5011);
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
 
   
  if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("connect error");
    exit(1);
  }

  str_cli(stdin, sockfd);		/* do it all */

  exit(0);
}

