/*************************************************************************
	> File Name: sigtest1.c
	> Author: lq
	> Created Time: 2019年07月14日 星期日 17时46分35秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid_arr[10];//globle var

int main()
{
	int i = 0;
	int pid;
	for (; i < 2; i++)
	{
		pid_arr[i] = pid = fork();
		if (pid == 0)
		{
			break;
		}
		else if (pid < 0)
		{
			perror("fork failed");
			exit(0);
		}
	}

	if (pid == 0 && i == 0 )
	{
		//first child
		exit(0);
	}

	if (pid == 0 && i == 1)
	{
		//second child
		exit(0);
	}
	
	//parent
	int mypid = 0;
	while ((mypid = waitpid(-1, NULL, 0) > 0))
	{
		printf("child %d existing\n", mypid);
	}
	printf("parent existing...\n");
	return 0;
}
