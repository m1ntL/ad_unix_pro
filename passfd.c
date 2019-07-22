#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

static const unsigned int CMSG_LENGTH = CMSG_LEN(sizeof(int)); 
void send_fd(int fd, int fd_to_send)
{
	struct msghdr msg;
	char buf[0];
	struct iovec iov[1]; 
	iov[0].iov_base = &buf;
	
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	
	struct cmsghdr cm;
	cm.cmsg_len = CMSG_LENGTH;
	cm.cmsg_level = SOL_SOCKET;
	cm.cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(&cm) = fd_to_send;
	
	msg.msg_control = &cm;
	sendmsg(fd, &msg, 0);
}

int recv_fd(int fd)
{
	struct msghdr msg;
	struct iovec iov[1];
	char buf[0];
	
	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	struct cmsghdr cm;
	//cm.cmsg_len = CMSG_LENGTH;
	//cm.cmsg_level = SOL_SOCKET;
	msg.msg_control = &cm;
	recvmsg(fd, &msg, 0);

	int fd_to_recv = *(int*)CMSG_DATA(&cm);
	return fd_to_recv;
}

int main()
{
	int pipe[2];
	int ret = socketpair(PF_UNIX, SOCK_DGRAM, 0, pipe);
	if (ret < 0 )
	{
		printf("create local socket failed!\n");
		return -1;
	}

	int pid = fork();
	if (pid < 0)
	{
		printf("fork failed!\n");
		return -1;
	}
	else if (pid == 0)
	{
		close(pipe[0]);
		int fd = open("/home/lq/mp/getaddrinfo.c", O_RDONLY); 
		send_fd(pipe[1], fd);
		close(fd);
		exit(0);
	}
	close(pipe[1]);
	char buf[BUFFER_SIZE];
	int fd2 = recv_fd(pipe[0]);
	recv(fd2, buf, BUFFER_SIZE-1, 0);
	printf("get %s from %d:", buf, fd2);
	return 0;
}
