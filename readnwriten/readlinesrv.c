/*************************************************************************
	> File Name: srv.c
	> Author: lq
	> Created Time: 2019年07月16日 星期二 17时09分46秒
	> Description: echo server with readline
				1, readn
				2, writen
				3, readline
				4, recv with MSG_PEEK
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

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;
	char *bufp = buf;
	ssize_t nleft = maxline;
	while (1)
	{
		ret = recv_peek(sockfd, bufp, nleft);
		if (ret < 0)//failed
			return ret;
		else if (ret == 0)//peer closed connection
			return ret;

		nread = ret;
		int i;
		for (i = 0; i < nread; i++)
		{
			if (bufp[i] =='\n')
			{
				ret = readn(sockfd, bufp, i+1); //read data
				if (ret != i+1)
					exit(EXIT_FAILURE);
				return ret; //read '\n' then return the data 
			}
		}

		if (nread > nleft)
		// the data read more than the max length of a line, exception
			exit(EXIT_FAILURE);

		// no "\n" in buffer, read all data recv(MSG_PEEK)
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			exit(EXIT_FAILURE);
		nleft -= nread;
		bufp += nread;
	}
	return -1;
}

void do_service(int connfd)
{
	char  recvbuf[1024];
	//struct packet sendbuf;
	//memset(&sendbuf, 0, sizeof(sendbuf));

	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = readline(connfd, recvbuf, 1024);
		if (ret == -1)
			ERR_EXIT("readline");
		if (ret == 0)
		{
			//not read entire length
			printf("client closed \n");
			break;
		}

		fputs(recvbuf, stdout);
		writen(connfd, recvbuf, strlen(recvbuf));
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
