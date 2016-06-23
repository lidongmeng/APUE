/*************************************************************************
	> File Name: ipc_mmap_write.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 16 Jun 2016 09:16:24 AM EDT
 ************************************************************************/

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

struct Student {
	char name[4];
	int age;
};

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage: ./write filename\n");
		return 0;
	}
	int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd == -1) ERR_EXIT("open error");

	lseek(fd, sizeof(Student)*5-1, SEEK_SET);
	write(fd, "", 1);

	Student * p = (Student*)mmap(NULL, sizeof(Student)*5, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == NULL) ERR_EXIT("mmap");

	char ch = 'a';
	for (int i = 0; i < 5; ++i) {
		memcpy((p+i)->name, &ch, 1);
		(p+i)->age = 20+i;
		ch++;
	}
	
	munmap(p, sizeof(Student)*5);
	return 0;
}
