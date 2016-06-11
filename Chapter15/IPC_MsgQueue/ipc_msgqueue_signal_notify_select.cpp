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
#include <sys/select.h>
#include <sys/types.h>
using namespace std;

#define MSG_QUEUE_NAME "/temp"
#define BUF_SIZE 1024
#define ERR_EXIT(m) \
	do {\
		perror(m); \
		exit(EXIT_FAILURE);\
	} while (0)

void sigusr1_handler(int signo);
int fds[2];

int main(int argc, char ** argv) {
	mqd_t mqd;

	if (pipe(fds) == -1) ERR_EXIT("pipe error");
	if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) ERR_EXIT("signal");

	char buf[1024];
	ssize_t n;
	int len;
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
	struct sigevent event;
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo = SIGUSR1;
	if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");

	// select read set
	fd_set rset;
	FD_ZERO(&rset);
	char c;
	int nfds;
	// write msg to the mqd util end
	while (1) {
		FD_SET(fds[0], &rset);
		nfds = select(fds[0] + 1, &rset, NULL, NULL, NULL);
		if (nfds == -1 && errno != EINTR) ERR_EXIT("select");
		if (FD_ISSET(fds[0], &rset)) {
			read(fds[0], &c, 1);
			if (mq_notify(mqd, &event) == -1) ERR_EXIT("mq_notify error");
			while ( (n = mq_receive(mqd, (char*)&buf, len, &prio)) >= 0)
				printf("receive message:[%s], prio:[%d]\n", buf, prio);
			if (errno != EAGAIN) ERR_EXIT("receive error");
		}
	}
	mq_close(mqd);
	return 0;
}

void sigusr1_handler(int signo) {
	write(fds[1], " ", 1);
	return ;
}
