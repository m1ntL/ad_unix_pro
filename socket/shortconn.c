/*************************************************************************
	> File Name: shortconn.c
	> Author: lq
	> Created Time: 2019年07月14日 星期日 22时43分29秒
	> Description: simple client with short connection, change from client.c 
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	for (int i = 0; i < 10; i++)
	{
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1)
		{
			perror("socket error");
			exit(0);
		}

		struct sockaddr_in srvaddr;
		srvaddr.sin_family = AF_INET;
		srvaddr.sin_port = htons(8045);
		srvaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	
		if (connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1)
		{
			perror("connect error");
			exit(0);
		}
		char recvbuf[1024];
		char sendbuf[1024];
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf, "i = %d\n", i);
		write(sockfd, sendbuf, sizeof(sendbuf));
		
		read(sockfd, recvbuf, sizeof(recvbuf));
		fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(sendbuf));
		close(sockfd);
	}
	
}
