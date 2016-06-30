/*************************************************************************
	> File Name: coroutine.h
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 30 Jun 2016 10:14:59 AM EDT
 ************************************************************************/

#include <ucontext.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define COROUTINE_DEAD 0
#define COROUTINE_REDY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3
typedef void (*coroutine_func)(struct Scheduler *, void * ud);

struct Coroutine;

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



Schedule * coroutine_open(void);
void coroutine_close(Schedule *);


int coroutine_new(Schedule * , coroutine_func, void * ud);
void coroutine_resume(Schedule*, int id);
int coroutine_status(Schedule*, int id);
int coroutine_running(Schedule *);
void coroutine_yield(Schedule*);
