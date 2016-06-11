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
#include <signal.h>
#include <pthread.h>
using namespace std;

#define MSG_QUEUE_NAME "/temp"
#define BUF_SIZE 1024
#define ERR_EXIT(m) \
	do {\
		perror(m); \
		exit(EXIT_FAILURE);\
	} while (0)

void notify_thread(union sigval arg);
mqd_t mqd;
struct sigevent event;
int len;
struct mq_attr attr;
int main(int argc, char ** argv) {
	int len;
	int ret;

	// create a mqd_t
	mqd = mq_open(MSG_QUEUE_NAME, O_RDONLY | O_NONBLOCK);
	if (mqd == -1) ERR_EXIT("mq_open");

	// get attr
	ret = mq_getattr(mqd, &attr);
	if (ret == -1) ERR_EXIT("mq_getattr");
	len = attr.mq_msgsize;
	printf("msg len: %d\n", len);
	struct sigevent event;
	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = notify_thread;
	event.sigev_notify_attributes = NULL;

	if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");

	// write msg to the mqd util end
	while (1) {
		pause();
	}
	mq_close(mqd);
	return 0;
}

void notify_thread(union sigval arg) {
	int n;
	void * buf;
	buf = malloc(attr.mq_msgsize);
	unsigned int prio;
	printf("notify_thread started\n");
	if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");
	while ( (n = mq_receive(mqd,(char*) buf, attr.mq_msgsize, &prio)) >= 0)
		printf("receive message:[%s], prio:[%d]\n", (char*)buf, prio);
	if (errno != EAGAIN) ERR_EXIT("receive error");
	pthread_exit(NULL);
}
