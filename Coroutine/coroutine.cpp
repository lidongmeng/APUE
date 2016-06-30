/*************************************************************************
	> File Name: coroutine.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 30 Jun 2016 10:19:26 AM EDT
 ************************************************************************/
#include "coroutine.h"

struct Schedule {
	char stack[1024*128];
	ucontext_t main;
	int numOfCoroutiines;
	int capacity;
	int running;
	Coroutine ** routines;
};

struct Coroutine {
	coroutine_func func;
	void * ud;
	ucontext_t ctx;
	Schedule * schdule;
	int capacity;
	int size;
	int status;
	char * stack;
};


static Coroutine * _co_new(Schedule * s, coroutine_func func, void * ud) {
	Coroutine * co = (Coroutine*)malloc(sizeof(Coroutine));
	co->func = func;
	co->ud = ud;
	co->schedule = s;
	co->capacity = 0;
	co->size = 0;
	co->status = COROUTINE_READY;
	co->stack = NULL;
}

static void _co_delete(Coroutine * co) {
	free(co->stack);
	free(co);
}

Schedule * coroutine_open(void) {
	Schedule * schedule = (Schedule*)malloc(sizeof(Schedule));
	schedule->numOfRoutines = 0;
	schedule->capacity = 128;
	schedule->running = -1;
	schedule->routines = malloc(sizeof(Coroutine*)*schedule->capacity);
	memset(schedule->routines, 0, sizeof(Coroutine*)*schedule->capacity);
	return schedule;
}

void coroutine_close(Schedule * s) {
	for (int i = 0; i < s->capacity; ++i) {
		Coroutine * co = s->routines[i];
		if (co != NULL) _co_delete(co); 
	}
	free(s->routines);
	s->routines = NULL;
	free(s);
}

int coroutine_new(Schedule * s, coroutine_func func, void * ud) {
	Coroutine * co = _co_new(s, func, ud);
	if (s->numOfRoutines >= s->capicity) {
		
	}
}

void coroutine_resume(Schedule*, int id) {

}

int coroutine_status(Schedule*, int id) {

}

int coroutine_running(Schedule *) {

}

void coroutine_yield(Schedule*) {

}
