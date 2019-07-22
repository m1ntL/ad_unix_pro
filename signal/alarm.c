/*************************************************************************
	> File Name: alarm.c
	> Author: lq
	> Created Time: 2019年07月08日 星期一 16时38分38秒
 ************************************************************************/
#include <unistd.h>
#include <signal.h>

void handler(int num)
{
	printf("rece signal %d\n", num);
	alarm(1);
}
int main()
{
	if (signal(SIGALRM, handler) == SIG_ERR)
	{
		perror("signal failed");
		exit(0);
	}
	
	alarm(1);
	while (1)
	{
		pause();
	}
	return 0;

}
