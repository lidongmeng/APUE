/*************************************************************************
	> File Name: ipc_fifo_read.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Wed 15 Jun 2016 09:09:59 AM EDT
 ************************************************************************/

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char ** argv) {
	if (argc != 3) {
		printf("usage: send fifofile cpfile\n");
		return 0;
	}
	int fifofd;

	mkfifo(argv[1], 0666);
	fifofd = open(argv[1], O_WRONLY); 
	if (fifofd == -1) ERR_EXIT("mkfifo");

	int fd = open(argv[2], O_RDONLY);
	char buf[1024];
	int n;
	while ((n = read(fd, buf, 1024)) != 0) {
		write(fifofd, buf,n);
	}
	close(fifofd);
	close(fd);
	return 0;
}
