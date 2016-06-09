/*************************************************************************
	> File Name: commom.h
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Wed 25 May 2016 06:18:49 AM PDT 
************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_


#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <pwd.h>
#include <shadow.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/sendfile.h>
#include <sys/un.h>
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

#endif //_COMMON_H_
