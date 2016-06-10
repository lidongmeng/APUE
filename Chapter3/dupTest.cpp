/*************************************************************************
	> File Name: cpTest.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Fri 10 Jun 2016 11:40:45 AM EDT
 ************************************************************************/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>

#define ERR_EXIT(m) \
	do {\
		perror(m);\
		exit(EXIT_FAILURE);\
	} while (0)

int main(int argc, char ** argv) {
	int fd;
	if (argc != 2) {
		printf("Usage: main pathname\n");
		return 0;
	}
	fd = open(argv[1], O_WRONLY);
	if (fd == -1) ERR_EXIT("open");
	//close(1);
	dup2(fd, 1);
	printf("hello\n");
	return 0;
}
