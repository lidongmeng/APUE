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
	int fd2;
	char buf[1024];
	int nread;
	if (argc != 2) {
		printf("Usag: main pathname\n");
		return 0;
	}

	infd = open(argv[1], O_RDWR);
	if (infd == -1) ERR_EXIT("open error");

	fd2 = open(argv[1], O_RDWR);
	if (fd2 == -1) ERR_EXIT("open error");
	
	while ((nread = read(infd, buf, sizeof(buf))) != 0) {
		buf[nread] = '\0';
		printf("%s\n", buf);
	}

	printf("Same file read again:\n");
	while ((nread = read(fd2, buf, sizeof(buf))) != 0) {
		buf[nread] = '\0';
		printf("%s\n", buf);
	}

	close(infd);
	close(fd2);
	return 0;
}
