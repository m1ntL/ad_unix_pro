/*************************************************************************
	> File Name: mulpro.c
	> Author: lq
	> Created Time: 2019年07月13日 星期六 16时10分45秒
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void test(int num)
{	
	printf("exec time: %d\n", num);
}

int main()
{
	int pro_num, loop_num;
	printf("input the num of process:");
	scanf("%d", &pro_num);
	printf("input the num of loop:");
	scanf("%d", &loop_num);

	
	for (int i = 0; i < pro_num; i++)
	{
		int pid = fork();
		if (pid == 0)
		{
			printf("child sequence: %d", i);
			printf("this is chlid %d\n", getpid());
			for (int j = 0; j < loop_num; j++)
			{
				test(j);
			}
			exit(0);
		}
		if (pid == -1)
		{	
			perror("fork failed");
			exit(0);
		}
		if (pid > 0)
		{	
			printf("this is parent %d\n", getpid());
			waitpid(pid, NULL,0 );
			
		}

	}	
	return 0;
	
}
