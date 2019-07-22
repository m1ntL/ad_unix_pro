#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	printf("before fork: %d\n", getpid());
		
	int fd = open("./1.txt", O_RDWR);
	if (fd == -1)
	{
		perror("my open");
		return -1;
	}
	int pid = fork();
	if (pid == -1)
	{
		perror("open err");
		exit(0);
	}
	else if (pid == 0)
	{
		write(fd, "child", 5);
		printf("this is child process %d, parent process is %d\n", getpid(), getppid());
		close(fd);
		exit(0);
	}
	else 
	{	
		sleep(2);
		write(fd, "parent", 6);
		printf("this is parent process %d\n", getpid());
		close(fd);
	}
	return 0;
}
