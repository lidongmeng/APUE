/*************************************************************************
	> File Name: ipc_msgqueue1.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Fri 10 Jun 2016 10:06:52 PM EDT
 ************************************************************************/

#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;

#define MSG_QUEUE_NAME "/temp"
#define BUF_SIZE 1024
#define ERR_EXIT(m) \
	do {\
		perror(m); \
		exit(EXIT_FAILURE);\
	} while (0)

int main(int argc, char ** argv) {
	mqd_t mqd;
	char buf[BUF_SIZE];
	char prioChar[10];
	size_t len;
	unsigned int prio;
	bool running = true;
	int ret;
	// set attr of mqd
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 1024;

	mq_unlink(MSG_QUEUE_NAME);
	// create a mqd_t
	mqd = mq_open(MSG_QUEUE_NAME, O_WRONLY | O_CREAT, 0666, &attr);
	if (mqd == -1) ERR_EXIT("mq_open");
	
	// write msg to the mqd util end
	while (running) {
		printf("Enter message: ");
		fgets(buf, BUF_SIZE, stdin);
		buf[strlen(buf)-1] = '\0';
		printf("Enter prio:");
		fgets(prioChar, 10, stdin);
		prio = atoi(prioChar);
		ret = mq_send(mqd, buf, sizeof(buf), prio);
		if (ret == -1) ERR_EXIT("mq_send");
		printf("write a msg to the queue successfully,msg: [%s], prio: [%d]\n", buf, prio);
		if (strncmp(buf, "end", 3) == 0) {
			printf("stop write msg, bye\n");
			running = 0;
		}
		sleep(1);
	}
	mq_close(mqd);
	return 0;
}

