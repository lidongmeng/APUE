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
using namespace std;

#define MSG_QUEUE_NAME "/temp"
#define BUF_SIZE 1024
#define ERR_EXIT(m) \
	do {\
		perror(m); \
		exit(EXIT_FAILURE);\
	} while (0)

bool running = true;
int len;
mqd_t mqd;
void sigusr1_handler(int signo);
struct sigevent event;
unsigned int prio;

int main(int argc, char ** argv) {
	// signal deal
	if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) ERR_EXIT("Signal error");
	
	int ret;
	// set attr of mqd
	struct mq_attr attr;

	// create a mqd_t
	mqd = mq_open(MSG_QUEUE_NAME, O_RDONLY);
	if (mqd == -1) ERR_EXIT("mq_open");

	// get attr
	ret = mq_getattr(mqd, &attr);
	if (ret == -1) ERR_EXIT("mq_getattr");
	len = attr.mq_msgsize;

	// sigevent notify regeister
	
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo = SIGUSR1;
	if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");

	// write msg to the mqd util end
	while (running) {
		pause();
	}
	mq_close(mqd);
	return 0;
}

void sigusr1_handler(int signo) {
	char buf[1025];
    if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");
	int ret = mq_receive(mqd, (char *)&buf, len, &prio);
	if (ret == -1) ERR_EXIT("mq_receive");
	printf("receive message [%s], prio:[%d]\n", buf, prio);
	if (strncmp(buf, "end", 3) == 0) running = false;
}
