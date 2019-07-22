/*************************************************************************
	> File Name: mulfork.c
	> Author: lq
	> Created Time: 2019年07月07日 星期日 22时24分53秒
 ************************************************************************/
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int loopnum = 0;
	int procnum = 0;
	printf("scanf the loopnum:\n");
	scanf("%d", &loopnum);
	printf("scanf the procnum\n");
	scanf("%d", &procnum);

	for (int i = 0; i < procnum; i++)
	{
		int id = fork();
		if (id == -1)
		{
			printf("fork failed\n");
		}
		if (id == 0)
		{
			for (int j = 0; j < loopnum; j++)
			{
				printf("%d----helloha \n", j);
			}
			exit(0);
		}
	}
	while (1)
	{
		int ret = wait(NULL);
		if (ret == -1)
		{
			if (errno == EINTR)
				continue;
			break;
		}
	}
	printf("all subprocess exited\n");
	return 0;

}
