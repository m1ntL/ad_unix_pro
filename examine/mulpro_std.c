/*************************************************************************
	> File Name: mulpro_std.c
	> Author: lq
	> Created Time: 2019年07月13日 星期六 16时51分41秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
void test(int num)
{
	printf("loop time: %d\n", num);
}

int main()
{
	int proc_num, loop_num;
	printf("input proc_num:");
	scanf("%d", &proc_num);
	printf("input loop_num:");
	scanf("%d", &loop_num);

	for (int i = 1; i <= proc_num; i++)
	{
		printf("current proc num = %d\n", i);
		int pid = fork();
		if (pid  == -1)
		{
			perror("fork failed");
			exit(0);
		}
		if (pid  == 0)
		{
			printf("this is child %d\n", getpid());
			for (int j = 1; j <= loop_num; j++)
			{
				test(j);
			}
			exit(0);
		}
	}
	printf("this is parent %d\n", getpid());

	/*
	int ret = 0;
	while (1)
	{
		ret = wait(NULL);
		if (ret == -1)
		{
			if (errno == EINTR)
				continue;
		}
		break;
	}
	*/
	int mypid = 0;
	while( (mypid = waitpid(-1, NULL, 0)) >0)
	{
		printf("exited child %d\n", mypid);
	}
	
	printf("parent exit.....\n");
	return 0;

}

