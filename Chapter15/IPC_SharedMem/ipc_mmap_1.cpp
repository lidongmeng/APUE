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
#define PAGE_SIZE 4096

int main(int argc, char ** argv) {
	int fd;
	int i;
	unsigned char * p_map;

	if (argc != 2) {
		printf("Usage: %s <filecontenttowirte>\n", argv[1]);
		exit(0);
	}
	
	fd = open("test.dat", O_RDWR|O_APPEND|O_CREAT);
	if (fd < 0) ERR_EXIT("open");

	p_map = (unsigned char*)mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (p_map == MAP_FAILED) ERR_EXIT("mmap");
	close(fd);

	for (i = 0; i < PAGE_SIZE; i++) {
		if (p_map[i] == 0) break;
	}

	printf("length:[%d]\n%s\n", i, p_map);
	char * line = argv[1];
	printf("content to be writen to file:%s\n", line);
	memcpy(p_map, line, strlen(line));
	printf("p_map:%s\n, %d\n", p_map, strlen(line));
	msync(p_map, strlen(line), MS_SYNC);
    
	close(fd);
	munmap(p_map, PAGE_SIZE);
	return 0;
}

