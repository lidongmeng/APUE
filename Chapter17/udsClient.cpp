/*************************************************************************
	> File Name: udsClient.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 09 Jun 2016 05:52:19 AM EDT
 ************************************************************************/

#define CLI_PATH "/var/tmp/"
#define CLI_PERM S_IRWXU
#include "common.h"

int client_connect(const char * name);

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage: main pathname\n");
		return 0;
	}
	int fd = client_connect(argv[1]);
	
	int nbytes;
	char buffer[256];

	nbytes = snprintf(buffer, 256, "hello from client");
	write(fd, buffer, nbytes);
	
	nbytes = read(fd, buffer, 256);
	buffer[nbytes] = 0;
	printf("receive from server:[%s]\n", buffer);
	close(fd);
	return 0;
}

int client_connect(const char * name) {
	int fd, len, rval;
	struct sockaddr_un sockAddr;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		ERR_EXIT("socket");
	}

	memset(&sockAddr, 0, sizeof(sockAddr));
	// fill socket address with server's address
	sockAddr.sun_family = AF_UNIX;
	strcpy(sockAddr.sun_path, name);

	if (connect(fd, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr_un)) < 0) {
		ERR_EXIT("connect");
	}

	return fd;
}
