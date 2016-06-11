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
	unsigned int prio;
	bool running = true;
	int ret;
	// set attr of mqd
	struct mq_attr attr;

	// create a mqd_t
	mqd = mq_open(MSG_QUEUE_NAME, O_RDONLY);
	if (mqd == -1) ERR_EXIT("mq_open");

	// get attr
	ret = mq_getattr(mqd, &attr);
	if (ret == -1) ERR_EXIT("mq_getattr");
	// write msg to the mqd util end
	while (running) {
		ret = mq_receive(mqd,(char*) &buf, attr.mq_msgsize, &prio);
		if (ret == -1) ERR_EXIT("mq_send");
		printf("receive a msg from the queue successfully,msg: [%s], prio: [%d]\n", buf, prio);
		if (strncmp(buf, "end", 3) == 0) {
			printf("msgqueue end, bye\n");
			running = 0;
		}
		sleep(1);
	}
	mq_close(mqd);
	return 0;
}

