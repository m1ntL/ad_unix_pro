#include <stdio.h>

extern char** environ;

int main()
{
	int i = 0;
	for (i = 0; environ[i] != NULL; i++)
	{
		printf("%s", environ[i]);
	}
	printf("getpid:%d\n", getpid());
	//pause();
	printf("hello....");
	return 0;
}
