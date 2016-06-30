/*************************************************************************
	> File Name: common.h
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 30 Jun 2016 11:36:17 AM EDT
 ************************************************************************/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)
