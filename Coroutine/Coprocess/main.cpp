/*************************************************************************
	> File Name: main.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 30 Jun 2016 11:37:25 AM EDT
 ************************************************************************/

#include "common.h"

int main(int argc, char ** argv) {
	int fd1[2], fd2[2];
	pid_t pid;
	size_t nwrite;
	size_t nread;
	char line[1024];

	if (pipe(fd1) == -1 || pipe(fd2) == -1) ERR_EXIT("pipe");
	pid = fork();

	if (pid < 0) {
		ERR_EXIT("fork");
	} else if (pid > 0) {
		close(fd1[0]);
		close(fd2[1]);

		while (fgets(line, 1024, stdin) != NULL) {
			if (line[0] == '\n') continue;
			nwrite = strlen(line);
			if (write(fd1[1], line, nwrite) != nwrite) ERR_EXIT("write");
			if ((nread = read(fd2[0], line, 1024)) < 0) ERR_EXIT("read");

			if (nread == 0) ERR_EXIT("child close pipe");

			line[nread] = '\0';
			printf("%s\n", line);
		}
		close(fd1[1]);
		close(fd2[0]);
		waitpid(pid, NULL, 0);
		exit(0);
	} else {
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) ERR_EXIT("dup2");
			close(fd1[0]);
		}
		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) ERR_EXIT("dup2");
			close(fd2[1]);
		}

		if (execl("./coprocess", "coprocess", (char*) NULL) < 0) ERR_EXIT("execl");
	}
}
