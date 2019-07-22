/*************************************************************************
	> File Name: twofork.c
	> Author: lq
	> Created Time: 2019年07月14日 星期日 14时42分35秒
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int pid = 0; 
	pid = fork();
	if (pid < 0)
	{
		perror("first fork failed");
		exit(0);
	}
	if (pid == 0)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("second fork failed");
			exit(0);
		}
		if (pid == 0)
		{
			//second child
			sleep(2);
			printf("this is second child, parent is %d\n", getppid());
			exit(0);
		}
		printf("first child exit.....\n");
		exit(0);
	}

	sleep(4);
	printf("this is parent\n");
	exit(0);
}
