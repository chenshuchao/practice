all: server client_lt client_et

server: 
	gcc -Wall -g tcpser.c -o server

client_lt:
	gcc -Wall -g tcpcli_epoll_lt.c -o client_lt

client_et:
	gcc -Wall -g tcpcli_epoll_et.c -o client_et

clean:
	rm server client_lt client_et

