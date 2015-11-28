#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <unistd.h>

int max ( int a, int b ) { return a > b ? a : b; }
const int MAXLINE = 1024;
void str_cli(FILE *fp, int sockfd)
{
  int maxfdp1;
  fd_set rset;
  char sendline[MAXLINE], recvline[MAXLINE];

  FD_ZERO(&rset);
  for ( ; ; ) {
    bzero(sendline, MAXLINE);
    bzero(recvline, MAXLINE);
    FD_SET(fileno(fp), &rset);
    FD_SET(sockfd, &rset);
    maxfdp1 = max(fileno(fp), sockfd) + 1;
    select(maxfdp1, &rset, NULL, NULL, NULL);

    if (FD_ISSET(sockfd, &rset))
    {	/* socket is readable */
	  if (read(sockfd, recvline, MAXLINE) < 0)
        perror("str_cli: server terminated prematurely");
	  fputs(recvline, stdout);
    }

	if (FD_ISSET(fileno(fp), &rset))  /* input is readable */
    {
      if (fgets(sendline, MAXLINE, fp) == NULL) return;		/* all done */
	  write(sockfd, sendline, strlen(sendline));
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

