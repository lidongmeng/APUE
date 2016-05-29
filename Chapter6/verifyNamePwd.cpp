/*************************************************************************
	> File Name: test.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Sat 28 May 2016 08:00:02 PM PDT
 ************************************************************************/
#include <shadow.h>
#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
using namespace std;


int main() {
	string username;
	string passwd;
	while (1) {
		cout << "Input username: ";
		cin >> username;
		struct passwd * pw = getpwnam(username.c_str());
		if (pw == NULL) {
			cout << "Incorrect user name" << endl;
			continue;
		}

		cout << "Input passwd: ";
		cin >> passwd;
		struct spwd * sp = getspnam(username.c_str());
		if (sp == NULL) {
			cout << "No such user name passwd in the shadow.h" << endl;
			continue;
		} else {
			cout << "encrypted pass: " << sp->sp_pwdp << endl;
			char * ep = crypt(passwd.c_str(), sp->sp_pwdp);
			cout << "hah: " << ep << endl;
			if (strcmp(ep, sp->sp_pwdp) == 0) {
				cout << "Login in correct" << endl;
			} else {
				cout << "Wrong passwd" << endl;
			}
		}
	}
	return 0;
}
