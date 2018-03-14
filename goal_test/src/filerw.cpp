#include <time.h>
#include <stdio.h>
#include <fstream>
#include "config.h"


using namespace std;
goal_test::alcohol_concentration sensor_data;

void sensor_subCallback(const goal_test::alcohol_concentration &msg)
{
    sensor_data.concentration_1 = msg.concentration_1;
    sensor_data.concentration_2 = msg.concentration_2;
    ROS_INFO("Concentartion 1 is %lf.",sensor_data.concentration_1); 
    ROS_INFO("Concentartion 2 is %lf.",sensor_data.concentration_2);
}

int main(int argc, char *argv[])
{
	time_t now ;
    struct tm *tm_now ;
    time(&now) ;
    tm_now = localtime(&now) ;
	char str[100];
	sprintf(str,"/home/luc/catkin_ws/src/goal_test/experiment_data/xingnengpingce.txt");
	ofstream  fout(str,ios::app);

	ros::init(argc, argv, "test_with_actionlib"); 

    //定义传感器订阅
    ros::NodeHandle n;
    ros::Subscriber sensor_sub;
    sensor_sub = n.subscribe("alcohol_concentration", 1, sensor_subCallback);
	fout<<"i"<<"\t"<<"concentration_1"<<"\t"<<"concentration_2"<<"\n";
	int i=1;
	ros::Rate loop_rate(5);
	while(ros::ok())
	{
		i++;
		ros::spinOnce();
		loop_rate.sleep();
		fout<<i<<"\t"<<sensor_data.concentration_1<<"\t"<<sensor_data.concentration_2<<"\n";
		
	}


	fout.close();
	
	return 0;
}