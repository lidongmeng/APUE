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
		// excute ls -l command and redirect the output to the pipe[1]
		dup2(fds[1], 1);
		close(fds[1]);
		close(fds[0]);
		execlp("ls", "ls", NULL);
	}
	dup2(fds[0], 0);
	close(fds[1]);
	close(fds[0]);
	execlp("wc", "wc", "-w", NULL);
	
	return 0;
}
