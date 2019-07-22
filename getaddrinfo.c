
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	struct addrinfo hints;
	struct addrinfo* res;

	bzero(&hints, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("192.168.103.138","daytime", &hints, &res);
	if (res != NULL)
	{
		char remote[INET_ADDRSTRLEN];
		printf("%d, %d, %d, %d, %d, %d %s ",res->ai_flags, res->ai_family, res->ai_socktype, res->ai_protocol,
			res->ai_addrlen, inet_ntop(AF_INET, res->ai_addr, remote, INET_ADDRSTRLEN)); 
	}
	freeaddrinfo(res);
}
