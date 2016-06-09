/*************************************************************************
	> File Name: udsServer.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 09 Jun 2016 06:09:19 AM EDT
 ************************************************************************/

#include "common.h"
#define QLEN 10

int server_listen(const char * name);

int connection_handler(int connection_fd) {
	int nBytes;
	char buffer[256];
	nBytes = read(connection_fd, buffer, 256);
	buffer[nBytes] = 0;
	printf("Message from client: [%s]\n", buffer);
	nBytes = snprintf(buffer, 256, "hello from the server");
	write(connection_fd, buffer, nBytes);
	close(connection_fd);
	return 0;
}


int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage: server pathname\n");
		return 0;
	}
	int listenfd = server_listen(argv[1]);
	
    int connection_fd;
	struct sockaddr_un un;
	socklen_t len = sizeof(un);
	while (1) {
		connection_fd = accept(listenfd, (struct sockaddr*)&un, &len);
		if (fork() == 0) {
			return connection_handler(connection_fd);
		}
		close(connection_fd);
	}

	close(listenfd);
	unlink(argv[1]);
	return 0;
}

int server_listen(const char * name) {
	int fd, len, rval;
	struct sockaddr_un un;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) ERR_EXIT("socket");

	unlink(name);
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);

	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

	if (bind(fd, (struct sockaddr*)&un, len) < 0) ERR_EXIT("bind");

	if (listen(fd, QLEN) < 0) ERR_EXIT("listen");

	return fd;
}
