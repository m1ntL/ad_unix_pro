/*************************************************************************
	> File Name: ipbase.c
	> Author: lq
	> Created Time: 2019年07月12日 星期五 10时41分08秒
 ************************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

int main()
{
	unsigned int data = 0x12345678;
	char *p = &data;
	printf("%x, %x, %x, %x\n", p[0], p[1], p[2], p[3]);
	if (p[0] == 0x78)
	{
		printf("little endian\n");
	}
	else 
		printf("big endian\n");
	printf("transfer host sequence to net sequence\n");
	/*
	 * #include <arpa/inet.h>
	 *
	 * uint32_t htonl(uint32_t hostlong);
	 *
	 *  uint16_t htons(uint16_t hostshort);
	 *
	 *  uint32_t ntohl(uint32_t netlong);
	 *
	 *  uint16_t ntohs(uint16_t netshort);
	 *
	 */
	unsigned int data1 = htonl(data);
	p = &data1;
	printf("%x, %x, %x, %x\n", p[0], p[1], p[2], p[3]);
	if (p[0] == 0x78)
	{
		printf("little endian\n");
	}
	else 
	{
		printf("big endian\n");
	}
	//return (0);
	/*
	 * #include <sys/socket.h>
	 * #include <netinet/in.h>
	 * #include <arpa/inet.h>
	 *
	 * int inet_aton(const char *cp, struct in_addr *inp); //
	 *
	 *in_addr_t inet_addr(const char *cp);//
	 *in_addr_t inet_network(const char *cp);
	 * char *inet_ntoa(struct in_addr in);//
	 * struct in_addr inet_makeaddr(int net, int host);
	 * in_addr_t inet_lnaof(struct in_addr in);
	 * in_addr_t inet_netof(struct in_addr in);
	 */
	/*
	 * Internet address. 
	 * struct in_addr {
	 *			uint32_t       s_addr;  // address in network byte order 
	 * };
	 */
	printf("test inet_aton():\n");
	struct in_addr *inp;
	inet_aton("192.168.103.138", inp);
	printf("%u\n", inp);
	printf("%u\n", inp->s_addr);
	
	printf("test inet_addr()\n");
	struct in_addr myaddr;
	myaddr.s_addr = inet_addr("192.168.103.138");
	printf("%u\n", myaddr.s_addr);

	printf("test inet_ntoa()\n");
	printf("%s\n", inet_ntoa(myaddr));
}
