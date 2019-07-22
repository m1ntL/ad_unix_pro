#include <unistd.h>

/*include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char * const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[],char *const envp[]);
*/
int main()
{
	printf("before exec:...........\n");	
	//PULL UP LS COMMAND PROGRAM
	//execlp("ls", "ls", "-l", NULL);
	
	//PULL UP HELLO PROGRAM
	// CHECK THE FD, observed that the two fds are identical.
	// PRINT THE ENVRIONMENT 
	/*
	printf("before exec....getpid: %d\n", getpid());
	execlp("./hello", NULL, NULL);
	*/

	//PULL UP HELLO PROGRAM AND FORWARD DEFAULT ENVIRONMENT
	char *const envp[] = {"aaa=111", "bbb=222", "ccc=333",  NULL};
	execle("./hello", NULL, envp);
	return 0;
}
