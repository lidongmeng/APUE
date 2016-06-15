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
	pid_t pid;
	int fds[2];
	int ret;

	ret = pipe(fds);
	if (ret == -1) ERR_EXIT("pipe");
	pid = fork();
	if (pid == -1) {
		ERR_EXIT("fork");
	} else if (pid == 0) { // child
		close(fds[0]); // close read end
		close(fds[1]); // close write end
		exit(EXIT_SUCCESS);
	}
	
	// close(fds[0]); // close read end
	// close all read end and write a message to the pipe
	// ret = write(fds[1], "hah", 3);
	// if (ret == -1) ERR_EXIT("write");
	close(fds[1]);
	char buf[5] = {0};
	ret = read(fds[0], buf, 5);
	printf("%d\n",ret);
	return 0;
}
