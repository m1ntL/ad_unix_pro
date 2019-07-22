/*************************************************************************
	> File Name: clt.c
	> Author: lq
	> Created Time: 2019年07月16日 星期二 14时54分55秒
	> Description: client of echo server
				1.	struct packet : len + buf
				2.	readn
				3.	writen 
				4. fgets() add "\n" automatically
 ************************************************************************/

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <errno.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

struct packet
{
	int len; // network byte order, strlen of buf
	char buf[1024];
};

ssize_t writen(int fd, const void* buf, ssize_t count)
{
	char *bufp = (char*)buf;
	ssize_t nwritten;
	ssize_t nleft = count;
	while(nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			//return  value!!!
			return -1;
		}
		else if (nwritten == 0)
			continue;
		bufp += nwritten;
		nleft -= nwritten;
	}
	return count;
}

ssize_t readn(int fd, void* buf, ssize_t count)
{
	char* bufp = (char*)buf;
	ssize_t readn;
	ssize_t leftn = count;

	while(leftn > 0)
	{
		if ((readn = read(fd, buf, leftn) ) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (readn == 0)
		{
			return count - leftn;
			// peer closed connection, return the number of already read.
			//printf("peer closed connection");
			//return -1;
		}
		 
		bufp += readn;
		leftn -= readn;
	}
	return count;
}

int main()
{
	struct sockaddr_in srvaddr;
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8050);
	srvaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		ERR_EXIT("socket error");
	if (connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0)
		ERR_EXIT("connect error");
	
	struct packet sendbuf;
	struct packet recvbuf;
	memset(&sendbuf, 0, sizeof(sendbuf));
	memset(&recvbuf, 0, sizeof(recvbuf));
	
	int n;
	while (fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin))
	{
		n = strlen(sendbuf.buf);
		sendbuf.len = htonl(n); 
		writen(sockfd, &sendbuf, n+4);

		int ret = readn(sockfd, &recvbuf.len, 4);
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < 4)
		{
			printf("peer closed connection\n");
			break;//out of while, close sock and exit
		}
		n = ntohl(recvbuf.len);
		ret = readn(sockfd, &recvbuf.buf, n);
		if (ret == -1)
		{
			ERR_EXIT("readn");
		}
		else if (ret < n)
		{
			printf("peer closed \n ");
			break;
		}
		fputs(recvbuf.buf, stdout);
		memset(&recvbuf, 0, sizeof(recvbuf));
		memset(&sendbuf, 0, sizeof(sendbuf));
	}//while
	close(sockfd);
	return 0;
}
