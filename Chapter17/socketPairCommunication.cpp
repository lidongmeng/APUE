/*************************************************************************
	> File Name: socketPairCommunication.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 09 Jun 2016 05:15:32 AM EDT
 ************************************************************************/

#include "common.h"
#define BUF_SIZE 30

int main() {
	int fd[2];
	char * str = "This is a test string";
	char * buf = (char*) calloc(1, BUF_SIZE);
	pid_t pid;

	int writeLen, readLen;

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
		ERR_EXIT("socketpair");
	}

	if ((pid = fork()) == -1) {
		ERR_EXIT("fork");
	} else if (pid > 0) {
		printf("parent pid:[%d]\n", getpid());
		close(fd[1]);
		if ((writeLen = write(fd[0], str, strlen(str))) == -1) {
			ERR_EXIT("parent write");
		}
	} else if (pid == 0) {
		printf("child pid:[%d]\n", getpid());
		close(fd[0]);
	}

	if ((readLen = read(fd[1], buf, BUF_SIZE)) == -1) {
		ERR_EXIT("read error");
	}

	printf("pid:[%d], buf: [%s]\n", getpid(), buf);
	return 0;
}
