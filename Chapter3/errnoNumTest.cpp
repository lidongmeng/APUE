/*************************************************************************
	> File Name: errnoNumTest.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Fri 10 Jun 2016 10:53:13 AM EDT
 ************************************************************************/
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>

int main(int argc, char ** argv) {
	int type = atoi(argv[1]);
	int ret = close(100);
	if (ret == -1){
		if (type == 1) 
			perror("close");
		else if (type == 2)
			fprintf(stderr, "close error with msg: %s\n", strerror(errno));
	}
	return 0;
}

