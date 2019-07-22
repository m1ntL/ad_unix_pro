/*************************************************************************
	> File Name: echosrv.c
	> Author: lq
	> Created Time: 2019年07月16日 星期二 22时34分59秒
	> Description: increased the handle for defunct of child when child exit
	1, readn 
	2, writen
	3, readline
	4, recv with MSG_PEEK
	5, resovle the problem of defunct of child: 
			(1) signal(SIGCHLD, SIG_IGN)
			(2) signal handler function with wait
			(3) signal handler function with waitpid
 ************************************************************************/
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>


#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

ssize_t readn(int fd, void *buf, ssize_t count)
{
	char *bufp = (char*)buf;
	ssize_t readn;
	ssize_t leftn = count;

	while (leftn > 0)
	{
		if ((readn = read(fd, buf, leftn)) < 0 )
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

ssize_t writen(int fd, const void *buf, ssize_t count)
{
	char* bufp = (char*)buf;
	ssize_t writen;
	ssize_t leftn = count;
	while(leftn > 0)
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

ssize_t recv_peek(int fd, void *buf, size_t len )
{
	while(1)
	{
		int ret = recv(fd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

ssize_t readline(int fd, void *buf, int maxlen)
{
	ssize_t ret;
	ssize_t nread;
	char *bufp = (char*)buf;
	ssize_t leftn = maxlen;

	while(1)
	{
		ret = recv_peek(fd, bufp, leftn);
		if (ret < 0) // error
			return ret;
		else if (ret == 0) // peer closed 
			return ret;

		nread = ret;
		for (int i = 0; i < nread; i++)
		{
			if (bufp[i] == '\n')
			{
				//find '\n', read all data before "\n" and return
				ret = readn(fd, bufp, i+1);
				if (ret != i+1)
					exit(EXIT_FAILURE);
				return ret;
			}
		}

		// don't find '\n', read all the data recv peeked
		ret = readn(fd, bufp, nread);
		if (ret != nread)
			exit(EXIT_FAILURE);
		leftn -= nread;
		bufp += nread;
	}
	return -1;
}

void echo_srv(int connfd)
{
	printf("echo_srv");
	char recvbuf[1024];
	while(1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = readline(connfd, recvbuf, 1024);
		if (ret == -1)
			ERR_EXIT("readline");
		else if (ret == 0)
		{

			printf("client closed connection\n");
			break;
		}
		fputs(recvbuf, stdout);
		writen(connfd, recvbuf, 1024);
	}
}

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenfd < 0)
		ERR_EXIT("socket error");
	int value = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &value, (socklen_t)sizeof(value)) < 0)
		ERR_EXIT("setsockopt error");
	struct sockaddr_in myaddr;
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8050);
	myaddr.sin_addr.s_addr = inet_addr("192.168.253.2");

	if (bind(listenfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0)
		ERR_EXIT("bind error");
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen error");
	
	while (1)
	{
		struct sockaddr_in cltaddr;
		socklen_t cltaddr_len;
		int connfd = accept(listenfd, (struct sockaddr*)&cltaddr, &cltaddr_len);
		if (connfd < 0)
			ERR_EXIT("accept error");
		/*
		struct sockaddr_in cltaddr;
		socklen_t cltaddr_len;
		*/
		memset(&cltaddr, 0, sizeof(cltaddr));
		if (getpeername(connfd, (struct sockaddr*)&cltaddr, &cltaddr_len) < 0)
			ERR_EXIT("getpeername ");
		printf("peer ip:%s, port:%d\n", inet_ntoa(cltaddr.sin_addr), ntohs(cltaddr.sin_port));
		int pid = fork();
		printf("after fork\n");
		if (pid < 0)
			ERR_EXIT("fork");
		if (pid == 0)
		{
			printf("this is child %d\n", getpid());
			printf("before echo_srv\n");
			//close(listenfd);
			//process event
			echo_srv(connfd);
			printf("child exit....\n");
			exit(EXIT_SUCCESS);
		}

		printf("this is parent\n");
		sleep(5);
		close(connfd);
		printf("parent exiting.....\n");
		exit(0);
	}
	close(listenfd);
	return 0;
}
