/*************************************************************************
	> File Name: sendsig.c
	> Author: lq
	> Created Time: 2019年07月08日 星期一 15时38分21秒
 ************************************************************************/
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void my_handler(int num)
{
	if (num == SIGINT)
	{
		printf("recv signal SIGINT\n");
	}
	if (num == SIGUSR1)
	{
		printf("recv signal SIGUSR1\n");
	}
	else 
	{
		printf("recv other signal\n");
	}
}

int main()
{
	printf("main begin.......\n");
	
	if (signal(SIGINT, my_handler) == SIG_ERR)
	{
		perror("signal failed for SIGINT");
		exit(0);
	}
	if (signal (SIGUSR1, my_handler) == SIG_ERR)
	{
		perror("signal failed for SIGUSR1");
		exit(0);
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(0);
	}

	if (pid == 0)
	{
		sleep(1);
		// send signal to parent 
		/*
		pid_t ppid = getppid();
		kill(ppid, SIGUSR1);
		*/

		//send signal to process group 1
		//kill(0, SIGUSR1);

		//send signal to process group 2
		pid_t pgid = getpgrp();
		killpg(pgid, SIGUSR1);
	}
	int n = 3;
	do
	{
		printf("parent begin sleepping\n");
		n = sleep(n);
		printf("the time left is %d\n", n);
	} while (n > 0);

	wait(NULL);
	return 0;
}
