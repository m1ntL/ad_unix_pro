/*************************************************************************
	> File Name: sigaction.c
	> Author: lq
	> Created Time: 2019年07月09日 星期二 09时30分34秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int num)
{
	printf("recv signal %d\n", num);
}

void handler2(int num, siginfo_t *info, void * p)
{
	printf("received signal %d\n", num);
}


int main()
{
	/*
	 *  int sigaction(int signum, const struct sigaction *act,
	 *                       struct sigaction *oldact);
	struct sigaction {
	               void     (*sa_handler)(int);
	               void     (*sa_sigaction)(int, siginfo_t *, void *);
	               sigset_t   sa_mask;
	               int        sa_flags;
	               void     (*sa_restorer)(void);
	           };
	*/
	struct sigaction sigact;
	sigact.sa_handler = handler;
		
//	sigaction(SIGINT, &sigact, NULL);
	
	sigact.sa_sigaction = handler2;
	sigaction(SIGINT, &sigact, NULL);

	while (1)
	{
		pause();
	}
	return 0;
}
