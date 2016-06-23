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
#include <semaphore.h>
using namespace std;

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)

#define NMESG 16
#define MESGSIZE 256

struct Message {
	sem_t mutex;
	sem_t nempty;
	sem_t nstored;
	int nput;
	long noverflow;
	sem_t noverflowmutex;
	long msgoff[NMESG];
	char msgdata[NMESG*MESGSIZE];
};
