#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

int max ( int a, int b ) { return a > b ? a : b; }
const int MAXLINE = 1024;
const int timeoutMs = 10000;
const int events_size = 128;


// et模式下读写设为非阻塞模式，否则循环读完数据后会阻塞。
int setnonblocking(int sfd)
{
  int old_opt = fcntl(sfd, F_GETFL);
  int new_opt = old_opt | O_NONBLOCK;
  fcntl(sfd, F_SETFL, new_opt);
  return 0;
}
void str_cli(FILE *fp, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  int epollfd = epoll_create1(EPOLL_CLOEXEC);
  struct epoll_event event1;
  bzero(&event1, sizeof(event1));
  event1.data.fd = sockfd;
  event1.events = EPOLLIN | EPOLLET;   // 设为 et 模式
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event1))
  {
    perror("epoll_ctl: sockfd");
  }
  setnonblocking(sockfd);

  printf("file fd: %d\n", fileno(fp));
  struct epoll_event event2;
  bzero(&event2, sizeof(event2));
  event2.data.fd = fileno(fp);
  event2.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fileno(fp), &event2) < 0)
  {
    perror("epoll_ctl: fp");
  }
  printf("file fd: %d\n", fileno(fp));
  setnonblocking(fileno(fp));
  
  struct epoll_event events[events_size];
  for ( ; ; )
  {
    int eventNums = epoll_wait(epollfd, events, events_size, timeoutMs);
    printf("after epoll\n");
    int i, ret = 0, n = 0;
    for (i = 0; i < eventNums; i ++)
    {
      ret = n = 0;
      // socket 可读, et 模式下这次事件不会重复触发, 必须循环全部读取出来
      if (events[i].data.fd == sockfd)
      {
        printf("sockfd ...\n");
        bzero(recvline, MAXLINE);
        while ((ret = read(sockfd, recvline+n, MAXLINE)) > 0)
        {
          n += ret;
          printf("after sockfd read, ret = %d, n = %d; ...\n", ret, n);
        }
	    if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        {
          perror("str_cli: server terminated prematurely");
	    }
        fputs(recvline, stdout);
      }
      // 终端数据可读
      else if (events[i].data.fd == fileno(fp))
      {
        printf("terminal ...\n");
        if (fgets(sendline, MAXLINE, fp) == NULL)
        {
          perror("fgets error");
        }
        n = strlen(sendline);
        while (n > 0)
        {
          ret = write(sockfd, sendline+ret, n);
          printf("after sockfd write, ret = %d, n = %d; ...\n", ret, n);
          if (ret < n)
          {
            if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
            {
              perror("str_cli: write error");
            }
            break;
          }
          n -= ret;
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

