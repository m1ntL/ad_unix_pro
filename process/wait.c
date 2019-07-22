/*************************************************************************
	> File Name: wait.c
	> Author: lq
	> Created Time: 2019年07月07日 星期日 20时36分30秒
 ************************************************************************/
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int pid = fork();
	if (pid == -1)
		{
			printf("fork failed");
			return -1;
		}
	if (pid == 0)
	{
		printf("this is child\n");
		//exit(100);
		abort();
	}
	printf("this is parent\n");
	int status;
	int ret = wait(&status);
	printf("the ret is %d, pid is %d\n", ret, getpid());
	if (WIFEXITED(status))
	{
		printf("%d\n", WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		printf("the signal is %d\n", WTERMSIG(status));
	}
	else if (WIFSTOPPED(status))
	{
		printf("the stop signal is %d\n", WSTOPSIG(status));
	}
	else 
	{
		printf("exited in other way\n");
	}
	return 0;
}
