/*************************************************************************
	> File Name: ipc_shm_client.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Wed 22 Jun 2016 11:17:54 AM EDT
 ************************************************************************/
#include "ipc_shm_cs.h"
#include<iostream>
using namespace std;

int main(int argc, char ** argv) {
	if (argc != 4) {
		printf("Usage: %s <shmfilepath> <sleepnusec> <loopnum>\n", argv[0]);
		exit(0);
	}

	char mesg[MESGSIZE];
	long offset;

	int nusec = atoi(argv[2]);
	int nloop = atoi(argv[3]);

	int fd = shm_open(argv[1], O_RDWR, 0666);
	if (fd == -1) ERR_EXIT("shm_open");

	Message * ptr = (Message*) mmap(NULL, sizeof(struct Message), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) ERR_EXIT("mmap");
	close(fd);

	for (int i = 0; i < nloop; ++i) {
		sleep(nusec);
		snprintf(mesg, MESGSIZE, "pid:[%ld],message:[%d]", (long)getpid(), i);
		printf("messge send: %s\n", mesg);
		// write message to shared memory
		// first check nempty if there is a empty slot
		if (sem_trywait(&ptr->nempty) == -1) {
			if (errno == EAGAIN) { // no empty slot already
				printf("Already full\n");
				sem_wait(&ptr->noverflowmutex);
				ptr->noverflow++;
				sem_post(&ptr->noverflowmutex);
				continue;
			} else {
				ERR_EXIT("sem_trywait");
			}
		}

		sem_wait(&ptr->mutex);
		offset = ptr->msgoff[ptr->nput];
		if (++(ptr->nput) >= NMESG) ptr->nput = 0;
		sem_post(&ptr->mutex);

		strcpy(&ptr->msgdata[offset], mesg);
		sem_post(&ptr->nstored);
	}
}
