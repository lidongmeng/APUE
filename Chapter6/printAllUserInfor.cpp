/*************************************************************************
	> File Name: test.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Sat 28 May 2016 08:00:02 PM PDT
 ************************************************************************/
#include <iostream>
#include <pwd.h>
#include <unistd.h>
using namespace std;


int main() {
	struct passwd * ptr;

	setpwent();
	while ((ptr = getpwent()) != NULL) {
		cout << ptr->pw_name << ":" << ptr->pw_passwd << ":"
			<< ptr->pw_uid << ":" << ptr->pw_gid << ":"
			<< ptr->pw_dir << ":" << ptr->pw_shell << endl;
	}
	endpwent();
	return 0;
}
