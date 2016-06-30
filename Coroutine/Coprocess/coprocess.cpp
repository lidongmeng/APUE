/*************************************************************************
	> File Name: coprocess.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 30 Jun 2016 11:30:56 AM EDT
 ************************************************************************/

#include "common.h"

int main(int argc, char ** argv) {
	int a, b;
	char line[1024];
	size_t nw;
	ssize_t nr;

	while ((nr = read(STDIN_FILENO, line, 1024)) > 0) {
		if (line[0] == '\n') continue;
		line[nr] = '\0';
		if (sscanf(line, "%d %d", &a, &b) == 2) {
			sprintf(line, "%d\n", a+b);
			nw = strlen(line);
			if (write(STDOUT_FILENO, line, nw) != nw) {
				ERR_EXIT("write");
			}
		} else {
			if (write(STDOUT_FILENO, "Invalid args\n", 13) != 13) {
				ERR_EXIT("write");
			}
		}
	}
	if (nr == 0) {
		ERR_EXIT("parent close pipe");
		exit(-1);
	}
	return 0;
}
