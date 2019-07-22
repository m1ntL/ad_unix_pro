/*************************************************************************
	> File Name: system.c
	> Author: lq
	> Created Time: 2019年07月07日 星期日 23时06分32秒
 ************************************************************************/
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int my_system(const char* command)
{
	if (command == NULL)
	{
		return 1;
	}
	pid_t pid ;
	int status;
	pid = fork();
	if (pid == -1)
	{
		printf("fork failed\n");
		return -1;
	}
	else if (pid == 0)
	{
		execl("/bin/sh", "sh", "-c", command, NULL);
		exit(127);
	}
	while(waitpid(pid, &status, 0) < 0)
	{
		if (errno == EINTR)
			continue;
		status = -1;
		break;
	}
	return status;
}
int main()
{
	int ret = my_system("ls -l");
	printf("ret is %d\n", ret);
	system("ls -l");
}
