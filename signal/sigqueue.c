/*************************************************************************
	> File Name: sigqueue.c
	> Author: lq
	> Created Time: 2019年07月09日 星期二 10时20分33秒
 ************************************************************************/
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void handler(int num, siginfo_t *st, void *p)
{
	if (num == SIGINT)
	{
		printf("recv signal %d\n", num);
		int myinfo = st->si_value.sival_int;
		printf("%d\n", myinfo);
	}
}
int main()
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(0);
	}
	if (pid == 0)
	{//send signal
		//int sigqueue(pid_t pid, int sig, const union sigval value);
		// union sigval {
		//                int   sival_int;
		//                void *sival_ptr;
		//              };
		sigval_t value;
		value.sival_int = 222;
		sigqueue(getppid(), SIGINT, value );
		sleep(1);
	}

	/*
	struct sigaction {
	               void     (*sa_handler)(int);
	               void     (*sa_sigaction)(int, siginfo_t *, void *);
	               sigset_t   sa_mask;
	               int        sa_flags;
	               void     (*sa_restorer)(void);
	           };*/
	//recv signal
	struct sigaction sigact;
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = handler;
	sigaction(SIGINT, &sigact, NULL);
// int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
	while (1)
	{
		pause();
	}

	return 0;
}
