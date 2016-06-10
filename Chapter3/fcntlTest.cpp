/*************************************************************************
	> File Name: fcntlTest.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Tue 31 May 2016 08:14:40 PM PDT
 ************************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#define ERR_EXIT(m) \
	do {\
		perror(m); \
		exit(EXIT_FAILURE); \
	}while (0)

void fcntlDup(int fd);
void fcntlGetFl(int fd);
void fcntlSetFl(int fd);

int main() {
	printf("Fcntl is used to operate file properties based on the file descriptor\n");
	int openFd = open("text.txt");

	return 0;
}
