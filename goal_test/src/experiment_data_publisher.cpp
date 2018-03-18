#include "config.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <goal_test/experiment_data.h>


//自定义消息类型，用于双传感器

goal_test::experiment_data experiment_data;

using namespace std;

void sensor_subCallback(const goal_test::alcohol_concentration &msg)
{
    experiment_data.concentration.concentration_1 = msg.concentration_1;
    experiment_data.concentration.concentration_2 = msg.concentration_2;
}



int main(int argc, char *argv[])
{
    //创建实验数据记录文件,并以日期命名
    time_t now ;
    struct tm *tm_now ;
    time(&now) ;
    tm_now = localtime(&now) ;
	char str[100];
	sprintf(str,"/home/luc/catkin_ws/src/goal_test/experiment_data/data-%d-%d-%d-%d-%d-%d.txt",
	tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	ofstream fout(str,ios::app);


    //初始化节点
    ros::init(argc, argv, "experiment_data_publisher"); 

    //定义传感器订阅
    ros::NodeHandle n;
    ros::Subscriber sensor_sub = n.subscribe("alcohol_concentration", 1, sensor_subCallback);
    ros::Publisher data_pub = n.advertise<goal_test::experiment_data>("experiment_data", 10);
    
    

    //定义tf相关常量
    tf::TransformListener listener;
    tf::StampedTransform transform;
    std::string odom_frame="/map";
    std::string base_frame="/base_footprint";//两个frame的名称很重要

    //tf坐标转换
    try
    {
        listener.waitForTransform(odom_frame, base_frame, ros::Time(), ros::Duration(2.0) );
    }
    catch(tf::TransformException& ex)
    {
        ROS_ERROR("%s", ex.what());
    }
    
    ros::Rate loopRate(10);

    while (ros::ok())
    {
        listener.lookupTransform(odom_frame,base_frame, ros::Time(0), transform);
        experiment_data.position.target_pose.header.frame_id = "map";
        experiment_data.position.target_pose.header.stamp = ros::Time::now();
        experiment_data.position.target_pose.pose.position.x = transform.getOrigin().x(); 
        experiment_data.position.target_pose.pose.position.y = transform.getOrigin().y();
        experiment_data.position.target_pose.pose.orientation.z = transform.getRotation().z();

        ros::spinOnce();

        data_pub.publish(experiment_data);

        fout<<experiment_data.position.target_pose.pose.position.x<<"\t"
            <<experiment_data.position.target_pose.pose.position.y<<"\t"
            <<experiment_data.position.target_pose.pose.orientation.z<<"\t"
            <<experiment_data.concentration.concentration_1<<"\t"
            <<experiment_data.concentration.concentration_2<<"\n";

        loopRate.sleep();
    }


    fout.close();  
}