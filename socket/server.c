/*************************************************************************
	> File Name: server.c
	> Author: lq
	> Created Time: 2019年07月12日 星期五 20时05分12秒
	> Description: simple echo server
					in setsockopt, we use SO_REUSEADDR support address multiplexing
 ************************************************************************/
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket failed");
		exit(0);
	}

//	int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
//  int setsockopt(int sockfd, int level, int optname, const void *optval,socklen_t optlen);
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		perror("set socket option failed");
		exit(0);
	}

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8045);
	myaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
	{
		perror("bind failed");
		exit(0);
	}

	if (listen(sockfd, SOMAXCONN) == -1)
	{
		perror("listen failed");
		exit(0);
	}
	
	struct sockaddr_in peeraddr;
	socklen_t peeraddr_len = sizeof(peeraddr);
	int connfd = accept(sockfd, (struct sockaddr*)&peeraddr, (socklen_t*)&peeraddr_len);
	if (connfd < 0)
	{
		perror("accept failed");
		exit(0);	
	}

	char buf[1024] = {0};
	while (1)
	{
		int ret = read(connfd, buf, sizeof(buf));
		if (ret == 0)
		{
			printf("peer has close the connection\n");
			exit(0);
		}
		else if (ret < 0)
		{
			perror("read failed");
			exit(0);
		}
		fputs(buf, stdout);
		write(connfd, buf, sizeof(buf));
	}

	close(connfd);
	close(sockfd);
	return 0;
}
