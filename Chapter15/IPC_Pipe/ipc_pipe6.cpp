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
#include <climits>
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

#define BUF_SIZE 68*1024

int main(int argc, char ** argv) {
	printf("PIPE_BUF:%d\n", PIPE_BUF);
	pid_t pid;
	int fds[2];
	int ret;

	char a[BUF_SIZE];
	char b[BUF_SIZE];
	memset(a, 'A', sizeof(a));
	memset(b, 'B', sizeof(b));

	ret = pipe(fds);
	if (ret == -1) ERR_EXIT("pipe");
	
	pid = fork();
	if (pid == 0) {
		close(fds[0]);
		ret = write(fds[1], a, sizeof(a));
		printf("pid:[%d], write %d bytes to the pipe\n", getpid(), ret);
		exit(0);
	}

	pid = fork();
	if (pid == 0) {
		close(fds[0]);
		ret = write(fds[1], b, sizeof(b));
		printf("pid:[%d], write %d bytes to the pipe\n", getpid(), ret);
		exit(0);
	}

	close(fds[1]);
	int fd = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	char buf[1024*4] = {0};
	int n = 1;
	while (1) {
		ret = read(fds[0], buf, sizeof(buf));
		if (ret == 0) break;
		printf("n=%02d pid=%d read %d bytes from pipe buf[4095]=%c\n", n++, getpid(), ret, buf[4095]);
		write(fd, buf, ret);
	}
	return 0;
}
