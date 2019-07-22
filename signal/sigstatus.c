/*************************************************************************
	> File Name: sigstatus.c
	> Author: lq
	> Created Time: 2019年07月08日 星期一 20时38分27秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler (int num )
{
	if (num == SIGINT)
		printf("recv signal %d\n", num);
	else if (num == SIGQUIT)
	{
		sigset_t uset;
		sigemptyset(&uset);
		sigaddset(&uset, SIGINT);

		sigprocmask(SIG_UNBLOCK, &uset, NULL);
		//signal(SIGINT, SIG_IGN);
	}
}

void printsigset(sigset_t * set)
{
	for (int i = 1; i < NSIG; i++ )
	{
		if (sigismember(set, i))
			putchar('1');
		else 
			putchar('0');
	}
	printf("\n");
}

int main()
{
	sigset_t pset;
	sigset_t bset;

	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);

	if (signal(SIGINT, handler) == SIG_ERR)
	{
		perror("signal sigint failed");
		exit(0);
	}
	if (signal(SIGQUIT , handler) == SIG_ERR)
	{
		perror("signal sigxxxs failed");
		exit(0);
		//ERR_EXIT("signal error");
	}

	sigprocmask(SIG_BLOCK, &bset, NULL);

	while (1)
	{
		sigpending(&pset);
		printsigset(&pset);
		sleep(1);
	}
	return 0;
}
