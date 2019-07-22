/*************************************************************************
	> File Name: bind.c
	> Author: lq
	> Created Time: 2019年07月12日 星期五 17时20分03秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>

//int socket(int domain, int type, int protocol);
int main()
{
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/*
	struct sockaddr_in {
	    sa_family_t    sin_family; // address family: AF_INET 
		in_port_t      sin_port;   // port in network byte order 
	    struct in_addr sin_addr;   // internet address
	};

	// Internet address. 
	struct in_addr {
		uint32_t       s_addr;     // address in network byte order 
	};
	*/		
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8001);
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//myaddr.sin_addr.s_addr = INADDR_ANY;

//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if ( bind(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	{
		perror("bind failed");
		exit(0);
	}
	return 0;
}
