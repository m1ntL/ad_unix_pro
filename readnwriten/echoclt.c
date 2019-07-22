/*************************************************************************
	> File Name: echoclt.c
	> Author: lq
	> Created Time: 2019年07月17日 星期三 13时14分56秒
	> Description: 
 ************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

ssize_t readn(int fd, void* buf, ssize_t count)
{
	char *bufp = (char*)buf;
	ssize_t readn;
	ssize_t leftn;
	while(leftn > 0)
	{
		if ((readn = read(fd, buf, leftn)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (readn == 0)
			return count - leftn;

		bufp += readn;
		leftn -= readn;
	}
	return count;
}

ssize_t writen(int fd, const void* buf, ssize_t count)
{
	char *bufp = (char*)buf;
	ssize_t writen;
	ssize_t leftn;
	while (leftn > 0)
	{
		if ((writen = write(fd, buf, leftn)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (writen == 0)
			continue;
		
		bufp += writen;
		leftn -= writen;
	}
	return count;
}

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
		ERR_EXIT("socket");
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_addr.s_addr = inet_addr("192.168.253.2");
	servaddr.sin_port = htons(8050);
	servaddr.sin_family = AF_INET;

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect");

	char sendbuf[1024] = {0};
	char recvbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf)-1, stdin) != NULL)
	{
		writen(sockfd, sendbuf, sizeof(sendbuf));
		
		readn(sockfd, recvbuf, sizeof(recvbuf));

		fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}

	close(sockfd);
	return 0;

	
}
