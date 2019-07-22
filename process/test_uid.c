
#include <unistd.h>
#include <stdio.h>

int main()
{
	int uid = getuid();
	int euid = geteuid();
	printf("the uid is %d, euid is %d", uid, euid);
}

