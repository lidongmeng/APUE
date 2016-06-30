/*************************************************************************
	> File Name: ucontextTest.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Thu 30 Jun 2016 09:43:49 AM EDT
 ************************************************************************/

#include<iostream>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <ucontext.h>
using namespace std;

void func(void * arg) {
	puts("1");
	puts("11");
	puts("1111");
}

void context_test() {
	char stack[1024*128];
	ucontext_t child, main;

	getcontext(&child);
	child.uc_stack.ss_sp = stack;
	child.uc_stack.ss_size = sizeof(stack);
	child.uc_stack.ss_flags = 0;
	child.uc_link = &main;

	makecontext(&child, (void(*)())func, 0);
	swapcontext(&main, &child);
	puts("main");
}

int main(int argc, char ** argv) {
	context_test();
	return 0;
}

