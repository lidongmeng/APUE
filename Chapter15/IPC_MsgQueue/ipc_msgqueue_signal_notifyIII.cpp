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

void sigusr1_handler(int signo);

volatile sig_atomic_t mqflag;

int main(int argc, char ** argv) {
	mqd_t mqd;
	char buf[1024];
	ssize_t n;
	int len;
	sigset_t zeromask, newmask, oldmask;
	unsigned int prio;
	int ret;
	// set attr of mqd
	struct mq_attr attr;

	// create a mqd_t
	mqd = mq_open(MSG_QUEUE_NAME, O_RDONLY | O_NONBLOCK);
	if (mqd == -1) ERR_EXIT("mq_open");

	// get attr
	ret = mq_getattr(mqd, &attr);
	if (ret == -1) ERR_EXIT("mq_getattr");
	len = attr.mq_msgsize;

	// sigevent notify regeister
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigemptyset(&oldmask);
	sigaddset(&newmask, SIGUSR1);

	signal(SIGUSR1, sigusr1_handler);
	struct sigevent event;
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo = SIGUSR1;
	if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");

	// write msg to the mqd util end
	while (1) {
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		while (mqflag == 0) sigsuspend(&zeromask);

		mqflag = 0;
    	if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");
		while ( (n = mq_receive(mqd, (char*)&buf, len, &prio)) >= 0)
			printf("receive message:[%s], prio:[%d]\n", buf, prio);
		if (errno != EAGAIN) ERR_EXIT("receive error");
		sigprocmask(SIG_UNBLOCK, &newmask, NULL);
	}
	mq_close(mqd);
	return 0;
}

void sigusr1_handler(int signo) {
	mqflag = 1;
	return ;
}
