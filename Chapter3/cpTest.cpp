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
	int infd;
	int outfd;
	if (argc != 3) {
		printf("Usag: main inpathname outpathname\n");
		return 0;
	}

	infd = open(argv[1], O_RDONLY);
	if (infd == -1) ERR_EXIT("open src error");

	outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfd == -1) ERR_EXIT("open dest error");

	char buf[1024];
	int nread;
	while ((nread = read(infd, buf, sizeof(buf))) > 0) {
		write(outfd, buf, nread);
	}
	close(infd);
	close(outfd);
	return 0;
}
