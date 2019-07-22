/*************************************************************************
	> File Name: unblock.c
	> Author: lq
	> Created Time: 2019年07月09日 星期二 13时45分35秒
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void printpendset()
{
	sigset_t pset;
	sigpending(&pset);
	for (int i = 1; i < NSIG; i++)
	{
		if (sigismember(SIGINT, &pset))
		{
			putchar('1');
		}
		else
			putchar('0');
	}
	printf("\n");
}

void handler(int num)
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
//	int sigaddset(sigset_t *set, int signum);
//	int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
	if (num == SIGUSR1)
	{
		printf("SIGUSR1: blocking sigint...\n");
/*		if (sigprocmask(SIG_BLOCK, &set, NULL) < 0)
		{
			perror("block SIGINT failed");
			exit(0);
		}
*/
		signal(SIGINT, SIG_IGN);
		printpendset();
	}

	if (num == SIGUSR2)
	{
		printf("SIGUSR2: unblocking sigint...\n");
		/*
		if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0)
		{
			perror("unblock SIGINT failed");
			exit(0);
		}
		*/
		signal(SIGINT, SIG_DFL);
		printpendset();
	}
	if (num == SIGINT)
	{
		printf("recv sigint %d\n", num);
		//signal(SIGINT, SIG_DFL);
	}
}

int main()
{
	if (signal(SIGUSR1, handler) == SIG_ERR)
	{
		perror("signal failed");
		exit(0);
	}

	if (signal(SIGUSR2, handler) == SIG_ERR)
	{
		perror("signal failed");
		exit(0);
	}

	if (signal (SIGINT, handler) == SIG_ERR)
	{
		perror("signal failed");
		exit(0);
	}
		
	char temp;
	while( (temp = getchar()))
	{
		if (temp == 'z')
			pause();
		else if (temp == '1')
		{
			kill(getpid(), SIGUSR1);
			continue;
		}
		
		else if (temp == '2')
		{
			raise(SIGUSR2);
			continue;
		}
			
	}
	
	printf("examine end\n");
	return 0;
}
