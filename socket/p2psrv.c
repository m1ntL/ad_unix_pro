/*************************************************************************
	> File Name: p2psrv.c
	> Author: lq
	> Created Time: 2019年07月15日 星期一 20时48分54秒
	> Description: p2p chatroom--server 
		1.  parent process tell child to quit if it detected the signal of peer close connection
		2.  parent process recv data from client and print on screen
		3.  chlid recv data from keyboard and send to client
		4. !!! when parent find the peer has closed and prepare to quit, it must notify child to quit!!!
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <signal.h>

void handler(int num)
{
	printf("recv signal %d\n", num);
	exit(0);
}

int main()
{
	//creat listen socket and bind and listen and accept connection
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
		perror("socket error");
		exit(0);
	}
	struct sockaddr_in myaddr;
	myaddr.sin_family =	AF_INET;
	myaddr.sin_port = htons(8045);
	myaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	if (bind(listenfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	{
		perror("bind error");
		exit(0);
	}
	
	if (listen(listenfd, SOMAXCONN) < 0)
	{
		perror("listen error");
		exit(0);
	}

	int connfd = 0;
	struct sockaddr_in cltaddr;
	socklen_t cltaddr_len = sizeof(cltaddr);
	if ((connfd = accept(listenfd, (struct sockaddr*)&cltaddr, &cltaddr_len)) < 0 )
	{
		perror("accept error");
		exit(0);
	}

	int pid = fork();
	if (pid  == -1)
	{
		perror("fork error");
		exit(0);
	}
	else if (pid == 0)
	{
		//child
		close(listenfd);
		char sendbuf[1024] = {0};
		signal(SIGUSR1, handler);
		while ( fgets(sendbuf, sizeof(sendbuf)-1, stdin) != NULL)
		{
			write(connfd, sendbuf, sizeof(sendbuf));
			memset(sendbuf, 0, sizeof(sendbuf));
		}
		close(connfd);
	}

	//parent 
	char recvbuf[1024] = {0};
	while (1)
	{
		int ret = 0;
		ret = read(connfd, recvbuf, sizeof(recvbuf));
		if (ret == 0)
		{
			printf("client has closed connection\n");
			//exit(0);
			break;
		}
		fputs(recvbuf, stdout);
		memset(recvbuf, 0, sizeof(recvbuf));
	}
	
	close(connfd);
	close(listenfd);
	kill(pid, SIGUSR1);
	sleep(1);
	exit(0);
}
