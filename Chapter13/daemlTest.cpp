/*************************************************************************
	> File Name: daemlTest.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Tue 28 Jun 2016 04:02:31 AM EDT
 ************************************************************************/

#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while (0)


int main(int argc, char ** argv) {
	pid_t pid;
	int ret, fd0, fd1, fd2;
	struct sigaction sa;
	
	pid = fork();
	if (pid == -1) { // error
		ERR_EXIT("fork");
	} else if (pid != 0) { // parent exit
		exit(0);
	}

	// begin a new session
	// the child process is sure not the process group leader
	setsid();

	// ensure future opens won't allocate controlling ttys
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) ERR_EXIT("can not ignore SIGHUP");

	// fork again
	pid = fork();
	if (pid < 0) {
		ERR_EXIT("fork");
	} else if (pid != 0) {
		exit(0);
	}

	// change directory to root directory
	if (chdir("/") < 0) ERR_EXIT("chdir");
	
	// clear file createion mask
	umask(0);

	// close all open files
	for (int i = 0; i < 1024; ++i) close(i);

	// redirect 0, 1, 2 to /dev/null
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	while (1) {
	}
	return 0;
}
