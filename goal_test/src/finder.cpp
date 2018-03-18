#include "config.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <goal_test/experiment_data.h>

goal_test::experiment_data array_of_expertiment_data[100];
goal_test::experiment_data temp_of_expertiment_data;
int counter;

//定义判断目标的是否在范围内的函数。
bool isInRange(double x,double y)
{
    //ROS_INFO("x=%f,y=%f",x,y);
    if((x>X_MIN)&&(x<X_MAX)&&(y>Y_MIN)&&(y<Y_MAX))
        return true;
    else
        {
            //ROS_INFO("The goal is not in the range.");
            return false;
        }
}

void sendGoal(MoveBaseClient& actionlibClient,MoveBaseGoal& goal)
{
    actionlibClient.sendGoal(goal);
    actionlibClient.waitForResult(ros::Duration(10.0));
    if(actionlibClient.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        {ROS_INFO("Goal reached.");successed_time++;}
    else
        ROS_INFO("Failed to reach the goal."); 
}

void assignGoal(move_base_msgs::MoveBaseGoal& goal,double x,double y){
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x=x; 
    goal.target_pose.pose.position.y=y;
    goal.target_pose.pose.orientation.z = 1.0;
}

void experiment_data_subCallback(const goal_test::experiment_data &msg)
{
    temp_of_expertiment_data = msg;
    ROS_INFO("Concentartion 1 is %lf.",temp_of_expertiment_data.concentration.concentration_1); 
    ROS_INFO("Concentartion 2 is %lf.",temp_of_expertiment_data.concentration.concentration_2);
}


int main(int argc, char *argv[])
{
    //创建实验数据记录文件,并以日期命名
    time_t now ;
    struct tm *tm_now ;
    time(&now) ;
    tm_now = localtime(&now) ;
	char str[100];
	sprintf(str,"/home/luc/catkin_ws/src/goal_test/experiment_data/goal-%d-%d-%d-%d-%d-%d.txt",
	tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	ofstream fout(str,ios::app);

    //初始化节点
    ros::init(argc, argv, "finder"); 

    //定义传感器订阅
    ros::NodeHandle n;
    ros::Subscriber experiment_data_sub;
    experiment_data_sub = n.subscribe("experiment_data", 1, experiment_data_subCallback);
    
    //定义actionlib的client
    MoveBaseClient ac("move_base", true);

    //等待action server响应
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

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
    
    
    //开始寻源
    for(counter=0;ros::ok();counter++)
    {   
        ROS_INFO("SEARCHING No.%d",counter); 
        
        ros::spinOnce();

    }
    fout.close();
    

}