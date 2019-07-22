#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//int daemon(int nochdir, int noclose)

int my_daemon(int nochdir, int noclose)
{
	int pid = fork();
	if (pid == -1)
	{
		perror("fork failed\n");
		return -1;
	}
	else if (pid > 0)
	{
		exit(0);
	}
	printf("pid:%d\n", getpid());
	int id = setsid();
	if (id == -1)
	{
		perror("setsid failed\n");
		return -1;
	}
	if (nochdir)
		chdir("/");
	if (noclose)
	{
		for (int i = 0; i < 3; i++)
		{	
			close(i);
		}

	}
	open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	printf("hello....\n");
	while(1)
	{
		sleep(1);
	}
}

int main()
{
	my_daemon(1,1);
}
