
//1 exit() and _exit()
/*
#include <unistd.h>
#include <stdio.h>

int main()
{
	printf("hello........");
	//exit(0);
	
	//fflush(stdout);
	//_exit(0);
}
*/

//2 atexit() check man atexit, it is a stack model, the function registered first is executed last
/*
#include <unistd.h>

void bye1()
{
	printf("bye1.....do\n");
}

void bye2()
{
	printf("bye2.........do\n");
}

int main()
{
	atexit(bye1);
	atexit(bye2);
	printf("hello, bp!\n");
	return 0;
}
*/

//3 abort

#include <unistd.h>
void bye()
{
	printf("bye1\n");
}
int main()
{
	atexit(bye);
	printf("hhhhhhhh\n");
	//abort();
} 
