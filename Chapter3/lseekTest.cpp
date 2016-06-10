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
	char buf[] = "abcdefjhkkkkkkk";
	if (argc != 2) {
		printf("Usag: main pathname\n");
		return 0;
	}

	infd = open(argv[1], O_RDWR);
	if (infd == -1) ERR_EXIT("open src error");

	printf("file id: %d\n", infd);
	if (lseek(infd, 1024, SEEK_SET) == -1) ERR_EXIT("lseek");
	printf("file id: %d\n", infd);

	if (write(infd, buf, 10) != 10) ERR_EXIT("write");

	close(infd);
	return 0;
}
