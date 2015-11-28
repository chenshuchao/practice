#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <unistd.h>
#include <sys/epoll.h>

int max ( int a, int b ) { return a > b ? a : b; }
const int MAXLINE = 1024;
const int timeoutMs = 10000;
const int events_size = 16;

void str_cli(FILE *fp, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];

  
  int epollfd = epoll_create(EPOLL_CLOEXEC);

  struct epoll_event event1;
  event1.data.fd = sockfd;
  event1.events = EPOLLIN;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event1);

  struct epoll_event event2;
  event2.data.fd = fileno(fp);
  event2.events = EPOLLIN;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fileno(fp), &event2);
  
  struct epoll_event events[events_size];
  for ( ; ; ) {
    bzero(sendline, MAXLINE);
    bzero(recvline, MAXLINE);
    
    int eventNums = epoll_wait(epollfd, events, events_size, timeoutMs);
    int i;
    for (i = 0; i < eventNums; i ++)
    {
      // socket 可读
      if (events[i].data.fd == sockfd)
      {
	    if (read(sockfd, recvline, MAXLINE) < 0)
        {
          perror("str_cli: server terminated prematurely");
	    } else {
          fputs(recvline, stdout);
        } 
      }
      // 终端数据可读
      else if (events[i].data.fd == fileno(fp))
      {
        if (fgets(sendline, MAXLINE, fp) == NULL) return;		/* all done */
	    if (write(sockfd, sendline, strlen(sendline)) < 0)
        {
          perror("str_cli: write error");
        }
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

