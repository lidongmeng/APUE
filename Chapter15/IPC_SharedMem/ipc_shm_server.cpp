/*************************************************************************
	> File Name: ipc_shm_server.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Wed 22 Jun 2016 11:18:08 AM EDT
 ************************************************************************/
#include "ipc_shm_cs.h"
#include<iostream>
using namespace std;

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage: %s <shmfilepath>\n", argv[0]);
		exit(0);
	}

	int index, lastoverflow, temp;
	long offset;
	shm_unlink(argv[1]);
	int fd = shm_open(argv[1], O_CREAT|O_RDWR, 0666);
	if (fd == -1) ERR_EXIT("shm_open");
	
	Message * ptr = (Message*) mmap(NULL, sizeof(struct Message), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	ftruncate(fd, sizeof(struct Message));
	close(fd);

	for (index = 0; index < NMESG; ++index) {
		ptr->msgoff[index] = index * MESGSIZE; 
	}

	sem_init(&ptr->mutex, 1, 1);
	sem_init(&ptr->nempty, 1, NMESG);
	sem_init(&ptr->nstored, 1, 0);
	sem_init(&ptr->noverflowmutex, 1, 1);

	index = 0;
	lastoverflow = 0;
	temp = 0;
	for ( ; ; ) {
		sleep(1);
		sem_wait(&ptr->nstored);
		sem_wait(&ptr->mutex);
		offset = ptr->msgoff[index];
		printf("index:[%d], message:[%s]\n", index, &ptr->msgdata[offset]);
		if (++index >= NMESG) index = 0;
		sem_post(&ptr->mutex);
		sem_post(&ptr->nempty);

		sem_wait(&ptr->noverflowmutex);
		temp = ptr->noverflow;
		sem_post(&ptr->noverflowmutex);
		if (temp != lastoverflow) {
			printf("noverflow = %d\n", temp);
			lastoverflow = temp;
		}
		
	}


	return 0;
}

