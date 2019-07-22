/*************************************************************************
	> File Name: srv.c
	> Author: lq
	> Created Time: 2019年07月16日 星期二 17时09分46秒
	> Description: echo server with readn and writen to handle the problem of tcp has no boundary.
				1. readn , read "len" first, then read "buf" according to it.
				2. writen, first write length into "len", then send entire packet including "len" and "buf" to socket
				3. struct packet as the buffer, increased len to indicate the length of data in buf.
 ************************************************************************/
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

struct packet
{
	int len;
	char buf[1024];
};

ssize_t readn(int fd, char* buf, ssize_t count)
{
	char *bufp = (char*)buf;
	ssize_t readn;
	ssize_t leftn = count;

	while (leftn >0)
	{
		if ((readn = read(fd, buf, leftn) ) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return -1;
		}
		else if (readn == 0)
			return count-leftn;

		bufp += readn;
		leftn -= readn;
	}
	return count;
}

ssize_t writen (int fd, const void* buf, ssize_t count)
{
	char *bufp = (char*)buf;
	ssize_t writen;
	ssize_t leftn = count;
	while (leftn > 0)
	{
		if ((writen = write(fd, buf, leftn)) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return -1;
		}
		else if (writen == 0)
			continue;
		
		bufp += writen;
		leftn -= writen;
	}
	return count;
}

void do_service(int connfd)
{
	struct packet recvbuf;
	//struct packet sendbuf;
	memset(&recvbuf, 0, sizeof(recvbuf));
	//memset(&sendbuf, 0, sizeof(sendbuf));

	while (1)
	{
		int ret = readn(connfd, (char*)&recvbuf.len, 4);
		if (ret == -1)
			ERR_EXIT("readn length  error");
		else if (ret < 4)
		{
			//not read entire length
			printf("client closed \n");
			break;
		}
		int n = ntohl(recvbuf.len);
		ret = readn(connfd, recvbuf.buf, n);
		if (ret == -1)
			ERR_EXIT("read data error");
		else if (ret < n)
		{
			printf("client closed \n");
			break;
		}

		fputs(recvbuf.buf, stdout);
		writen(connfd, &recvbuf, n+4);
		memset(&recvbuf, 0, sizeof(recvbuf));
	}
}

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		ERR_EXIT("socket");

	struct sockaddr_in myaddr;
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8050);
	myaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	if (bind(listenfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0)
		ERR_EXIT("bind");
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	/*
	struct sockaddr_in cltaddr;
	socklen_t cltaddr_len = sizeof(cltaddr);
	int connfd;
	 */
	while(1)
	{
		struct sockaddr_in cltaddr;
		socklen_t cltaddr_len = sizeof(cltaddr);
		int connfd = accept(listenfd, (struct sockaddr*)&cltaddr, &cltaddr_len);
		if (connfd == -1)
			ERR_EXIT("accept");
		printf("ip:%s, port:%d\n", inet_ntoa(cltaddr.sin_addr) , ntohs(cltaddr.sin_port));
		int pid = fork();
		if (pid  == -1)
			ERR_EXIT("fork");
		if (pid == 0)
		{
			close(listenfd);
			//process service
			do_service(connfd);
			//close(connfd);
			exit(0);
		}
		
		//parent
		close(connfd);

	}
	close(listenfd);
	return 0;
}
