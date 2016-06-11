/*************************************************************************
	> File Name: timeTest.cpp
	> Author: lidongmeng
	> Mail: lidongmeng@ict.ac.cn
	> Created Time: Fri 03 Jun 2016 12:33:16 AM PDT
 ************************************************************************/
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
using namespace std;


int main() {

	// time_t time(time_t * tloc) --get time in seconds
	time_t timeInSeconds = time(NULL);
	cout << timeInSeconds << endl;
	// use localtime to convert the time_t to year-mon-day-hour-min-sec format
	struct tm * timeOfFullFormat = gmtime(&timeInSeconds); // = localtime(&timeInSeconds);
	cout << "UTC: " << (timeOfFullFormat->tm_year + 1900) << "-" << (timeOfFullFormat->tm_mon + 1) << "-"
		 << timeOfFullFormat->tm_mday << "-" << timeOfFullFormat->tm_hour << "-"
		 << timeOfFullFormat->tm_min << "-" << timeOfFullFormat->tm_sec << endl; 

	// use gettimeofday to get time in seconds and microseconds
	struct timeval tv;
	if (gettimeofday(&tv, NULL) == -1) {
		cout << "gettimeofday error" << endl;
	} else {
		cout << tv.tv_sec << " " << tv.tv_usec << endl;
	}
	struct tm * timeOfFullFormatII = localtime(&tv.tv_sec);
	cout << "Localtime: " << (timeOfFullFormatII->tm_year + 1900) << "-" << (timeOfFullFormatII->tm_mon + 1) << "-"
		 << timeOfFullFormatII->tm_mday << "-" << timeOfFullFormatII->tm_hour << "-"
		 << timeOfFullFormatII->tm_min << "-" << timeOfFullFormatII->tm_sec << endl; 

	// get exectute time of the process
	clock_t processExcetuteTime = clock();
	cout << processExcetuteTime / CLOCKS_PER_SEC  << endl;
	sleep(2);
	processExcetuteTime = clock();
	cout << processExcetuteTime / CLOCKS_PER_SEC << endl;

	// use tm struct to construct a time_val
	timeOfFullFormatII->tm_hour = 16;
	timeOfFullFormatII->tm_min = 16;
	tv.tv_sec = mktime(timeOfFullFormatII);
	if (settimeofday(&tv, NULL) == -1) cout << "settimeofday error" << endl;
	
	if (gettimeofday(&tv, NULL) == -1) {
		cout << "gettimeofday error" << endl;
	} else {
		cout << tv.tv_sec << " " << tv.tv_usec << endl;
	}
	timeOfFullFormatII = localtime(&tv.tv_sec);
	cout << "Localtime: " << (timeOfFullFormatII->tm_year + 1900) << "-" << (timeOfFullFormatII->tm_mon + 1) << "-"
		 << timeOfFullFormatII->tm_mday << "-" << timeOfFullFormatII->tm_hour << "-"
		 << timeOfFullFormatII->tm_min << "-" << timeOfFullFormatII->tm_sec << endl; 

	// print the format of time
	time(&timeInSeconds);
	cout << ctime(&timeInSeconds) << endl;
	cout << asctime(localtime(&timeInSeconds)) << endl;
	return 0;
}
