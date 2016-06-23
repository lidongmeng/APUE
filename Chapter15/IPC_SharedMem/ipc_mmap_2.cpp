/*************************************************************************
	> File Name: ipc_mmap_1.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Wed 22 Jun 2016 09:32:58 AM EDT
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cstring>
using namespace std;

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char ** argv) {
	int fd;
	int i;
	unsigned char * ptr;

	if (argc != 4) {
		printf("Usage: %s <filename> <filesize> <mmapsize>\n", argv[1]);
		exit(0);
	}

	int filesize = atoi(argv[2]);
	int mmapsize = atoi(argv[3]);
	fd = open(argv[1], O_RDWR|O_TRUNC|O_CREAT, 0666);
	if (fd < 0) ERR_EXIT("open");
	lseek(fd, filesize-1, SEEK_SET);
	write(fd, " ", 1);

	ptr = (unsigned char*)mmap(NULL, mmapsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) ERR_EXIT("mmap");
	close(fd);

	int pagesize = sysconf(_SC_PAGESIZE);
	printf("pagesize:[%d]\n", pagesize);

	for (i = 0; i < max(filesize, mmapsize); i += pagesize) {
		printf("ptr[%d] = %d\n", i, ptr[i]);
		ptr[i] = 1;
		printf("ptr[%d] = %d\n", i + pagesize-1, ptr[i+pagesize-1]);
		ptr[i+pagesize-1] = 1;
	}
	printf("ptr[%d] = %d\n", i, ptr[i]);
	return 0;
}

