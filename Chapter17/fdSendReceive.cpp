/*************************************************************************
	> File Name: fdSendReceive.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 09 Jun 2016 07:59:14 AM EDT
 ************************************************************************/

#include "common.h"

void send_fd(int fd, int fd_to_send);
int recv_fd(int fd);

int main(int argc, char ** argv) {
	int sockfds[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfds) < 0) ERR_EXIT("sockpair");
	pid_t pid;
	pid = fork();
	if (pid > 0) {
		close(sockfds[1]);
		int fd = recv_fd(sockfds[0]);
		char buf[1024] = {0};
		read(fd, buf, sizeof(buf));
		printf("buf=%s\n", buf);
	} else if (pid == 0) {
		close(sockfds[0]);
		int fd = open("test.txt", O_RDONLY);
		if (fd == -1) ERR_EXIT("open");
		send_fd(sockfds[1], fd);
	}
	return 0;
}


void send_fd(int fd, int fd_to_send) {
	int ret;
	struct msghdr msg;
	struct cmsghdr * p_cmsg;
	struct iovec vec;
	char cmsgbuf[CMSG_SPACE(sizeof(fd_to_send))];
	int * p_fds;
	char sendchar = 0;

	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	p_cmsg = CMSG_FIRSTHDR(&msg);
	p_cmsg->cmsg_level = SOL_SOCKET;
	p_cmsg->cmsg_type = SCM_RIGHTS;
	p_cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_send));
	p_fds = (int*)CMSG_DATA(p_cmsg);
	*p_fds = fd_to_send;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;

	vec.iov_base = &sendchar;
	vec.iov_len = sizeof(sendchar);
	ret = sendmsg(fd, &msg, 0);
	if (ret == -1) ERR_EXIT("sendmsg");
}

int recv_fd(int fd) {
	int ret;
	struct msghdr msg;
	char recvchar;
	struct iovec vec;
	int recv_fd;
	char cmsgbuf[CMSG_SPACE(sizeof(recv_fd))];
	struct cmsghdr * p_cmsg;
	int * p_fd;

	vec.iov_base = &recvchar;
	vec.iov_len = sizeof(recvchar);
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	msg.msg_flags = 0;

	p_fd = (int *) CMSG_DATA(CMSG_FIRSTHDR(&msg));
	*p_fd = -1;
	ret = recvmsg(fd, &msg, 0);
	if (ret == -1) ERR_EXIT("recvmsg");

	p_cmsg = CMSG_FIRSTHDR(&msg);
	if (p_cmsg == NULL) ERR_EXIT("recvmsg");

	p_fd = (int*)CMSG_DATA(p_cmsg);
	recv_fd = *p_fd;
	return recv_fd;
}
