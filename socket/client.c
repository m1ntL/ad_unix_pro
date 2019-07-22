/*************************************************************************
	> File Name: client.c
	> Author: lq
	> Created Time: 2019年07月12日 星期五 20时48分17秒
	> Description: simple client for  echo server 
 ************************************************************************/
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd  == -1)
	{
		perror("create socket failed ");
		exit(0);
	}

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8045);
	servaddr.sin_addr.s_addr = inet_addr("192.168.253.2");
	socklen_t servaddr_len = sizeof (servaddr);
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("connect with server failed");
		exit(0);
	}
	char recvbuf[1024] = {0};
	char sendbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		write(sockfd, sendbuf, sizeof(sendbuf));

		read(sockfd, recvbuf, sizeof(recvbuf));

		fputs(recvbuf, stdout);
		
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
		
	}
	
	close(sockfd);
	return 0;
	
}
