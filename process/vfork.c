#include <unistd.h>

int main()
{
	printf("before fork: %d\n", getpid());
	//int abc = 10;
	int ret = 0;
	int pid = vfork();
	if (pid == -1)
	{
		perror("fork failed!");
		return -1;
	}
	else if (pid == 0)
	{
		printf("this is child: %d, parent is %d\n", getpid(), getppid());
		//printf("abc=%d\n", abc);
		
		//PULL COMMAND PROGRAM
		char *const argv[] = {"ls", "-l", NULL};
		ret = execve("/usr/bin/ls", argv, NULL);
		//PULL YOUR PROGRAM
		//ret = execve("hello", NULL, NULL);
		if (ret == -1)
		{
			perror("execve:error");
		}
		printf("after execve......");
	}
	else 
	{
		printf("this is parent: %d\n", getpid());
	}
	//printf("after fork");
	return 0;
}
