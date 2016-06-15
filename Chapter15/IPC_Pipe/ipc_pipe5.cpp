/*************************************************************************
	> File Name: ipc_pipe1.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Tue 14 Jun 2016 10:24:39 PM EDT
 ************************************************************************/

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (1)

int main(int argc, char ** argv) {
	int fds[2];
	int ret;

	ret = pipe(fds);
	if (ret == -1) ERR_EXIT("pipe");
	
	int flag = fcntl(fds[1], F_GETFL);
	fcntl(fds[1], F_SETFL, flag | O_NONBLOCK);
	int count = 0;
	while (1) {
		ret = write(fds[1], "A", 1);
		if (ret == -1) {
			perror("write");
			break;
		}
		count++;
	}
	printf("maxCnt:%d\n", count);
	return 0;
}
