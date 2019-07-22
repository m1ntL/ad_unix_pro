/*************************************************************************
	> File Name: 8.6wait.c
	> Author: lq
	> Created Time: 2019年07月13日 星期六 19时24分42秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void pr_exit(int status)
{
	if (WIFEXITED(status))
	{
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));		
	}
	else if (WIFSIGNALED(status))
	{
		printf("terminated by signal, exit status = %d\n", 
				WTERMSIG(status),
#ifdef WCOREDUMP
				WCOREDUMP(status) ? "(core file generated:)": "");
#else
				"");
#endif
	}
	else if (WIFSTOPPED(status))
	{
		printf("child stopped,signal number= %d\n", WSTOPSIG(status));
	}

}

int main()
{
	int status;
	int pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(0);
	}
	else if (pid == 0)
		exit(7);

	if (wait(&status) != pid)
	{
		perror("wait error");
		exit(0);
	}
	pr_exit(status);

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		exit(0);
	}
	else if(pid == 0)
		abort();
	if (wait(&status) != pid)
	{
		perror("wait error");
		exit(0);
	}
	pr_exit(status);

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		exit(0);
	}
	else if(pid == 0)
		status /= 0;
	if (wait(&status) != pid)
	{
		perror("wait error");
		exit(0);
	}
	pr_exit(status);

	exit(0);
}
