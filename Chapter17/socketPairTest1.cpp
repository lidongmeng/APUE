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

	int writeLen, readLen;

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
		ERR_EXIT("socketpair");
	}

	// write to fd[0] and read from fd[1]
	printf("write to fd[0]: [%s]\n", str);
	if ((writeLen = write(fd[0], str, strlen(str))) == -1) ERR_EXIT("write");

	if ((readLen = read(fd[1], buf, BUF_SIZE)) == -1) ERR_EXIT("read");

	printf("read from fd[1]: [%s]\n", buf);

	if ((readLen = read(fd[0], buf, BUF_SIZE)) == -1) ERR_EXIT("read");
	printf("read from fd0: [%s]\n", buf);
	return 0;
}
