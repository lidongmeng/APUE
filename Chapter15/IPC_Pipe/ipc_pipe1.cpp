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
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char ** argv) {
	pid_t pid;
	int fds[2];
	int ret;

	ret = pipe(fds);
	if (ret == -1) ERR_EXIT("pipe");
    // printf("pid:[%d]\n", getpid());	
	pid = fork();
	if (pid == -1) {
		ERR_EXIT("fork");
	} else if (pid == 0) { // child
		close(fds[0]);
		char buff[5] = "1234";
		ret = write(fds[1], buff, 5);
		if (ret != 5) ERR_EXIT("write");
		close(fds[1]);
		exit(EXIT_SUCCESS);
	} else {	
		close(fds[1]);
		char buf[5];
		ret = read(fds[0], buf, 5);
		printf("process:[%d],buf:[%s]\n", getpid(), buf);
		close(fds[0]);
	}
	return 0;
}
