#include <time.h>
#include <stdio.h>
#include <fstream>
using namespace std;
int main()
{
	time_t now ;
    struct tm *tm_now ;
    time(&now) ;
    tm_now = localtime(&now) ;
	char str[100];
 	double f=14.309948;
	sprintf(str,"/home/luc/catkin_ws/src/goal_test/experiment_data/%d-%d-%d-%d-%d-%d.txt",
	tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	ofstream fout(str);
	fout<<"hehe";
	fout.close();
	
	return 0;
}