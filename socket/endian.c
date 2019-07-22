/*************************************************************************
	> File Name: endian.c
	> Author: lq
	> Created Time: 2019年07月11日 星期四 21时42分47秒
	> Description: two ways to judge the byte order of current machine
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

typedef union _test
{
	unsigned short value;
	//char arr[2];
	char arr[sizeof(unsigned short)];
}test_struct;

int main()
{
	unsigned int data = 0x12345678;
	printf("sizeof(unsigned int)=%ld\n", sizeof(unsigned int));
	printf("sizeof(int)=%ld\n", sizeof(int));
	char *num = &data;
	printf("%x, %x, %x, %x\n", num[0], num[1], num[2], num[3]);
	if (num[0] == 0x78)
	{
		printf("little endian\n");
	}
	else if (num[0] == 0x12)
	{
		printf("big endian\n");
	}
	else
	{
		printf("unkonwn\n");
	}
	printf("*************************\n");
	test_struct test;
	test.value = 0x1234;
	if (test.arr[0] == 0x12)
	{
		printf("big endian\n");
	}
	else 
	{
		printf("little endian\n");
	}

	return 0;
	
}
