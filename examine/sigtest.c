/*************************************************************************
	> File Name: sigtest.c
	> Author: lq
	> Created Time: 2019年07月14日 星期日 14时50分42秒
	> Description: child1 send signal with info to child2, child2 send signal and info to parent.
 ************************************************************************/
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void handler(int num, siginfo_t* info)
{
	if (num == SIGRTMIN)
	{
		printf("2:recv SIGRTMIN from child1\n");
		printf("2:info : %s\n", (char*)info->si_value.sival_ptr);
		if (sigqueue(getppid(), SIGRTMIN, info->si_value) == -1)
		{
			perror("2:child2 send to parent error");
			exit(0);
		}
		printf("2:send SIGRTMIN to parent, child2 existing\n");
		exit(0);
	}
}

void pr_sig(int num, siginfo_t* info)
{
	if (num == SIGRTMIN)
	{
		printf("p:recv sig %d, info :%s\n", num, (char*)(info->si_value.sival_ptr));
	}
}
int main()
{
	//parent 
	//construct a pipe to send pid2 to child1
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		exit(0);
	}
	int pid1 = fork();
	if (pid1 < 0)
	{
		perror("first fork failed");
		exit(0);
	}
	if (pid1 == 0)
	{
		//child 1
		// get pid2 from parent
		close(pipefd[1]);
		pid_t c2id;
		//ssize_t read(int fd, void *buf, size_t count);
		if (read(pipefd[0], &c2id, sizeof(c2id)) == -1)
		{
			perror("read error");
			exit(0);
		}
		close(pipefd[0]);
		sleep(2);
		//send reliable signal to child2 with info
		//		int sigqueue(pid_t pid, int sig, const union sigval value);
		/*union sigval {
		 *            int   sival_int;
		 *            void *sival_ptr;
		 *          };
		 */
		union sigval val;
		//int sendif = getpid() *2;
		//val.sival_int = sendif;
		
		val.sival_ptr = "hello, im child1";
		if (sigqueue(c2id, SIGRTMIN, val) < 0)
		{
			perror("1:send signal to child2 error");
			exit(0);
		}
		printf("1:child1 %d existing....\n", getpid());
		exit(0);

	}
	
	
	int pid2 = fork();
	if (pid2 < 0)
	{
		perror("second fork failed");
		exit(0);
	}
	else if (pid2 == 0)
	{
		// child2
		// recv signal from child1 and send to parent
		/*int sigaction(int signum, const struct sigaction *act,
		 *                      struct sigaction *oldact);
		 *  struct sigaction {
		 *		void     (*sa_handler)(int);
		 *		void     (*sa_sigaction)(int, siginfo_t *, void *);
		 *		sigset_t   sa_mask;
		 *		int        sa_flags;
		 *		void     (*sa_restorer)(void);
		 *	};
		 */
		struct sigaction sigact;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_flags = SA_SIGINFO;
		sigact.sa_sigaction = handler;
		if ( sigaction(SIGRTMIN, &sigact, NULL) == -1)
		{
			perror("child2 sigaction error ");
			exit(0);
		}
		pause();
		printf("2:child2 exiting.....\n");
		exit(0);
	}

	//parent
	//send pid2 to child1
//	ssize_t write(int fd, const void *buf, size_t count)
	close(pipefd[0]);
	if (write(pipefd[1], &pid2, sizeof(pid2)) < 0)
	{
		perror("write to pipe failed");
		exit(0);
	}
	close(pipefd[1]);	
	//recv and print signal from child2
	struct sigaction psa;
	sigemptyset(&psa.sa_mask);
	psa.sa_flags = SA_SIGINFO;
	psa.sa_sigaction = pr_sig;
	if (sigaction(SIGRTMIN, &psa, NULL) == -1)
	{
		perror("parent sigaction error");
		exit(0);
	}
	pause();
	printf("p:info transfer ending...\n");
	return 0;
}
