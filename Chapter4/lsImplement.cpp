/*************************************************************************
	> File Name: lsImplement.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Sat 28 May 2016 10:23:43 PM PDT
 ************************************************************************/
#include <sys/types.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
using namespace std;

bool aFlag = false;
bool lFlag = false;
bool pathFlag = false;
string pathname = "";

void parseParameter(string s) {
	int index = 0, n = s.size(), startIndex = 0;
	// parse paramter
	int cnt = 0;
	while (index < n) {
		while (index < n && s[index] == ' ') ++index;
		startIndex = index;
		while (index < n && s[index] != ' ') ++index;
		string temp = s.substr(startIndex, index-startIndex);
		if (temp.empty()) break;
		if (cnt == 0) {
			cnt++;
		} else {
			if (temp[0] == '-' && temp[1] == 'a') {
				aFlag = true;
			} else if (temp[0] == '-' && temp[1] == 'l') {
				lFlag = true;
			} else {
				pathFlag = true;
				pathname = temp;
			}
		}
	}
}

void printInfoOfFile(const char * filename) {
	struct stat buf;
	if (!lFlag) {
		char * p = (char*)&filename[strlen(filename)-1];
		while (*p != '/') --p;
		++p;
		cout << p << "\t";
		return ;
	}
	if (lstat(filename, &buf) < 0) {
		perror("lstat");
		exit(-1);
	}

	// judge file type
	switch(buf.st_mode & S_IFMT) {
		case S_IFREG: // regular file
			cout << "-";
			break;
		case S_IFDIR:
			cout << "d";
			break;
		case S_IFCHR: // character special file
			cout << "c";
			break;
		case S_IFBLK: // block file
			cout << "b";
			break;
		case S_IFIFO: // pipe
			cout << "p";
			break;
		case S_IFLNK: // link file
			cout << "l";
			break;
		case S_IFSOCK: // socket
			cout << "s";
			break;
		default:
			break;
	}

	// print r w x
	for (int i = 8; i >= 0; --i) {
		// judge every bit of the last 8 bit of st_mode
		if (buf.st_mode & (1 << i)) {
			if (i%3 == 0) {
				cout << "x";
			} else if (i%3 == 1) {
				cout << "r";
			} else {
				cout << "x";
			}
		} else {
			cout << "-";
		}
	}

	cout << "\t" << buf.st_nlink << "\t";
	
	// get user name by user id
	struct passwd * pw = getpwuid(buf.st_uid);
	cout << pw->pw_name << "\t";
	// get group name by group id
	struct group * gp = getgrgid(buf.st_gid);
	cout << gp->gr_name << "\t";

	// bytes of file
	cout << buf.st_size << "\t";

	// last access time
	struct tm * time = localtime(&buf.st_atime);
	cout << (time->tm_year + 1900) << "-" << time->tm_mon + 1 << "-" 
		<< time->tm_mday << "-" << time->tm_hour << ":"
		<< time->tm_min << "\t";
	char * p = (char*)&filename[strlen(filename) - 1];
	while (*p != '/') --p;
	++p;
	cout << p;
	
	char out[100];
	if (S_ISLNK(buf.st_mode)) {
		cout << "-> ";
		if (readlink(filename, out, 100) == -1) {
			cout << "error link";
		} else {
			cout << out;
		}
	}
	cout << endl;
}

void do_ls() {
	if (pathname == "") pathname = "./";
	struct stat buf;
	DIR * dir;
	char filename[1024];


	if (lstat(pathname.c_str(), &buf) < 0) {
		perror("lstat");
		exit(-1);
	}

	if (S_ISDIR(buf.st_mode)) {
		dir = opendir(pathname.c_str());
		struct dirent *pdir;
		while ( (pdir = readdir(dir)) != NULL) {
			if (!aFlag) {
				if (pdir->d_name[0] == '.') continue;
			}
	        memset(filename, 0, sizeof(filename));
			strcpy(filename, pathname.c_str());
			strcat(filename, "/");
			strcat(filename, pdir->d_name);
			printInfoOfFile(filename);
		}
	} else {
		printInfoOfFile(pathname.c_str());
	}
}

int main(int argc, char ** argv) {
	string s;
	for (int i = 0; i < argc; ++i) {
		s += argv[i];
		s += ((i == argc - 1) ? "" : " ");
	}
    parseParameter(s);
    do_ls();
	cout << endl;
	return 0;
}
