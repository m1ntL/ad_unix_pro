/*************************************************************************
	> File Name: sigexm.c
	> Author: lq
	> Created Time: 2019年07月12日 星期五 23时30分44秒
 ************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#define BUFFSIZE 20480
 
void handler(int num)
{
	if (num == SIGINT || num == SIGQUIT)
	{
		printf("recv signal %d\n", num);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int main()
{
	if (signal(SIGINT, handler) == SIG_ERR)
	{
		perror("register SIGINT failed");
		exit(0);
	}
	
	if ( signal(SIGQUIT, handler) == SIG_ERR)
	{
		perror("register SIGQUIT failed");
		exit(0);
	}

//	int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
	sigset_t bset;
	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);
	sigaddset(&bset, SIGQUIT);
	
	sigset_t oldset;
	sigemptyset(&oldset);
	sigprocmask(SIG_BLOCK, &bset, &oldset);

	printf("open file\n");
	int fd = open("myfile", O_RDONLY);
	if (fd == -1)
	{
		perror("open file failed");
		exit(0);
	}
	printf("before read\n");
	char buf[BUFFSIZE];
	while(read(fd, buf, sizeof(buf)) != 0)
	{
		fputs(buf, stdout);
		sleep(5);
	}
	//ssize_t read(int fd, void *buf, size_t count);
	/*if (fread(fd, stdout, BUFFSIZE) == -1)
	{
		printf("read error %d", errno);
		exit(0);
	}
	*/
	printf("read end\n");
	printf("unblock signal\n");
	sigprocmask(SIG_UNBLOCK, &bset, NULL);
	while (1)
	{
		pause();
	}
	return 0;
}
