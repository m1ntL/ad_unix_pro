/*************************************************************************
	> File Name: p2pclt.c
	> Author: lq
	> Created Time: 2019年07月15日 星期一 20时21分37秒
	> Description:  p2p chat room-client 
			*  1. parent process recv data from keyboard and send to server
			*  2. child process recv data from server and print at screen
			*  3. child process tell parent to quit if detected the peer has closed the connection
			*  4. !!! read error handle !!!!!
			*  5. !!! in general, the signal registered before fork system call  !!!! 
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

void handler(int num)
{
	printf("recv signal %d\n", num);
	exit(0);
}
int main()
{
	//socket and connect 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket error");
		exit(0);
	}
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8045);
	srvaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	if( connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1)
	{
		perror("connect error");
		exit(0);
	}
	
	signal(SIGUSR1, handler);
	int pid = fork();
	if (pid  == -1)
	{
		perror("fork error");
		exit(0);
	}
	else if (pid == 0){
		//child
		printf("this is child : %d\n", getpid());
		char recvbuf[1024] = {0};
		while (1)
		{
			//char recvbuf[1024] = {0};
			int ret = read(sockfd, recvbuf, sizeof(recvbuf));
			if (ret == 0)
			{
				printf("server has closed connect\n");
				break;
			}
			else if (ret < 0)
			{
				perror("read error");
				break;
			}
			fputs(recvbuf, stdout);
			memset(recvbuf, 0, sizeof(recvbuf));
		}
		close(sockfd);
		kill(getppid(), SIGUSR1);
		exit(0);
	}
	// parent
	char sendbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf)-1, stdin) != NULL )
	{
		write(sockfd, sendbuf, sizeof(sendbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
	}
	
	close(sockfd);
	return 0;
}
