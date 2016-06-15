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
    // printf("pid:[%d]\n", getpid());	
	pid = fork();
	if (pid == -1) {
		ERR_EXIT("fork");
	} else if (pid == 0) { // child
		close(fds[0]);
		char buff[5] = "1234";
		sleep(3);
		ret = write(fds[1], buff, 4);
		if (ret != 4) ERR_EXIT("write");
		close(fds[1]);
		exit(EXIT_SUCCESS);
	}
	
	close(fds[1]);
	char buf[10] = {0};
	// fcntl set the read O_NONBLOCK
	int flags = fcntl(fds[0], F_GETFL);
	fcntl(fds[0], F_SETFL, flags | O_NONBLOCK);
	do {
		ret = read(fds[0], buf, 10);
		if (ret == -1) perror("read");
	} while (ret == -1 && errno == EAGAIN);
	printf("process:[%d],buf:[%s]\n", getpid(), buf);
	close(fds[0]);
	return 0;
}
