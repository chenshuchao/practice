### epoll lt 模式

启动server

    ./server_lt localhost 5011

    telnet localhost 5011

发送超过 10 个字符，epoll　触发了两次。


### epoll et 模式

    ./server_et localhost 5012

    telnet localhost 5011


发送超过 10 个字符，epoll　触发了一次。

