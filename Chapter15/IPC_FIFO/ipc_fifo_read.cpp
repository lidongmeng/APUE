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
	if (argc != 2) {
		printf("usage: read pathname\n");
		return 0;
	}
	int ret;

	ret = open(argv[1], O_RDONLY); // | O_NONBLOCK); 
	if (ret == -1) ERR_EXIT("open");

	printf("open success\n");
	return 0;
}
