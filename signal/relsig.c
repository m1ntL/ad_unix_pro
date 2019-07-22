/*************************************************************************
	> File Name: relsig.c
	> Author: lq
	> Created Time: 2019年07月09日 星期二 21时38分41秒
 ************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void handler2(int num, siginfo_t * info, void *p)
{
	if (num == SIGUSR1)
	{
		sigset_t ubset;
		sigemptyset(&ubset);
		sigaddset(&ubset, SIGINT);
		sigaddset(&ubset, SIGRTMIN);

		if (sigprocmask(SIG_UNBLOCK, &ubset, NULL ) == -1)
		{
			perror("unblock failed");
			exit(0);
		}
	}
	else if (num == SIGINT || num == SIGRTMIN)
	{
		int if_num = info->si_int;
		printf("recv signal %d \n", num);
		printf("and recv data : %d\n",if_num );
	}
	else 
	{
		printf("recv other siganl %d\n", num);
	}
}

void handler(int num) 
{
	if (num == SIGUSR1)
	{
		sigset_t ubset;
		sigemptyset(&ubset);
		sigaddset(&ubset, SIGINT);
		sigaddset(&ubset, SIGRTMIN);

		if (sigprocmask(SIG_UNBLOCK, &ubset, NULL) == -1)
		{
			printf("sigprocmask failed, errno=%d\n", errno);
			exit(0);
		}
	}
	else if (num == SIGINT || num == SIGRTMIN)
	{
		printf("recv signal %d\n", num);
	}
	else 
	{
		printf("recv signal %d\n", num);
	}
}


int main()
{
	struct sigaction sigact;
//	sigact.sa_sigaction = handler;
	
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = handler2;
	if (sigaction(SIGINT, &sigact, NULL) == SIG_ERR)
	{
		perror("SIGINT register failed");
		exit(0);
	}
	if (sigaction(SIGRTMIN, &sigact, NULL) == SIG_ERR)
	{
		perror("SIGRTMIN register failed");
		exit(0);
	}

	if (sigaction(SIGUSR1, &sigact, NULL) == SIG_ERR)
	{
		perror("SIGUSR1 register faild");
		exit(0);
	}
	
	sigset_t bset;
	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);
	sigaddset(&bset, SIGRTMIN);
	int ret = sigprocmask(SIG_BLOCK, &bset, NULL);
	if (ret == -1 )
	{
		perror("set block mask failed");
		exit(0);
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork failed ");
		exit(0);
	}
	if (pid == 0)
	{
		pid_t ppid = getppid();
		for (int i = 0; i < 3 ; i++)
		{
			union sigval value;
			value.sival_int = 222;
//		 int sigqueue(pid_t pid, int sig, const union sigval value);
			if (sigqueue(ppid, SIGINT, value) == -1)
			{
				perror("send SIGINT failed");
				exit(0);
			}
			else 
			{
				printf("send SIGINT ok\n");
			}
		}

		for (int i = 0; i < 3 ; i++)
		{
			union sigval val;
			val.sival_int = 3333;
			if (sigqueue(ppid, SIGRTMIN, val) == -1)
			{
				perror("send SIGRTMIN failed");
				exit(0);
			}
			else
			{
				printf("send SIGRTMIN ok\n");
			}
		}

		kill (ppid, SIGUSR1);
			
	}
	while (1)
	{
		pause();
	}
	return 0;
}
