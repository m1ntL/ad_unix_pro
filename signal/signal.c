/*************************************************************************
	> File Name: signal.c
	> Author: lq
	> Created Time: 2019年07月07日 星期日 23时58分38秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int num)
{
	printf("receive num %d\n", num);
	if (num == SIGQUIT)
	{//	exit(0);
	}
}

int main1()
{
	pid_t pid = fork();
	if (pid == -1)
	{
		printf("fork failed");
		exit(0);
	}
	if (pid == 0)
	{
		printf("this is child\n");
		exit(0);
	}

	signal(SIGCHLD, SIG_IGN);
	while (1)
	{
		pause();
	}

	return 0;
}

int main2()
{
	char tempchar;
	signal(SIGINT, handler);
	while ((tempchar = getchar())!= 'a')
	{
		pause();
	}
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		pause();
	}
	return 0;
}

//test the return value
int main()
{
	char tempchar;
	if (signal(SIGINT, handler) == SIG_ERR)
	{
		perror("signal err");
		exit(0);
	}

	/*
	__sighandler_t old = signal(SIGINT, handler);
	if (SIG_ERR != old)
	{
		perror("signal failed\n");
		exit(0)；
	}*/
	//printf("getchar\n");
	while ((tempchar = getchar()) != 'a')
	{
		pause();
	}
	
	//signal(SIGINT, old);
	//printf("recover the signal state\n");
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		pause();
	}
}
