#include "config.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <fstream>


//自定义消息类型，用于双传感器
goal_test::alcohol_concentration sensor_data;

using namespace std;

int quadrantCaculator(float z,goal_test::alcohol_concentration  concentration)//concentration_1 在左
//计算出的四个象限，之后在计算increasement时，使机器人按相应的象限的方向移动
{
    int quadrant;
    ROS_INFO("z=%f,concentration_1=%f,concentration_2=%f",z,concentration.concentration_1,concentration.concentration_2);
    if(z<-0.707)
        quadrant=3;
    else if(z<0)
        quadrant=4;
    else if(z<0.707)
        quadrant=1;
    else if(z<=1)
        quadrant=2;

    ROS_INFO("The origin quadrant is %d",quadrant);
    if(concentration.concentration_1>concentration.concentration_2)
        {
            if(quadrant==4)
                quadrant=1;
            else
                quadrant+=1;
        }
    else        
        {
            if(quadrant==1)
                quadrant=4;
            else
                quadrant-=1;
        }
    ROS_INFO("The transformed quadrant is %d",quadrant); 
    return quadrant; 
}

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
    goal.target_pose.header.frame_id = "odom";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x=x; 
    goal.target_pose.pose.position.y=y;
    goal.target_pose.pose.orientation.z = 1.0;
}

void sensor_subCallback(const goal_test::alcohol_concentration &msg)
{
    sensor_data.concentration_1 = msg.concentration_1;
    sensor_data.concentration_2 = msg.concentration_2;
}



int main(int argc, char *argv[])
{
    //创建实验数据记录文件,并以日期命名
    time_t now ;
    struct tm *tm_now ;
    time(&now) ;
    tm_now = localtime(&now) ;
	char str[100];
	sprintf(str,"/home/luc/catkin_ws/src/goal_test/experiment_data/%d-%d-%d-%d-%d-%d.txt",
	tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	ofstream fout(str);


    //初始化节点
    ros::init(argc, argv, "test_with_actionlib"); 

    //定义传感器订阅
    ros::NodeHandle n;
    ros::Subscriber sensor_sub;
    sensor_sub = n.subscribe("alcohol_concentration", 1, sensor_subCallback);
    
    //定义actionlib的client
    MoveBaseClient ac("move_base", true);

    //等待action server响应
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    //定义tf相关常量
    tf::TransformListener listener;
    tf::StampedTransform transform;
    std::string odom_frame="/odom";
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

    //获取开始位置
    /*listener.lookupTransform(odom_frame,base_frame, ros::Time(0), transform);
    float x_start = transform.getOrigin().x();
    float y_start = transform.getOrigin().y();

    //起点赋值（必须全部赋值，否则发送目标将失败）
    goal[0].target_pose.header.frame_id = "odom";
    goal[0].target_pose.header.stamp = ros::Time::now();
    goal[0].target_pose.pose.position.x = x_start;
    goal[0].target_pose.pose.position.y = y_start;
    goal[0].target_pose.pose.orientation.z = 1.0;*/

    //发送坐标
    //sendGoal(ac,goal[0]);//此处会立即返回已到达目标点，因为goal[0]即是起点

    //获取酒精浓度
    //ros::spinOnce();
    //current_concentration[0]=sensor_data;
    
    //定义一些中途变量
    int i,quadrant;
    float x_current,y_current,z_current,x_increasement,y_increasement,x_goal,y_goal;
    
    //将表头写入文件
    fout<<"i"<<"\t"<<"x_goal"<<"\t"<<"y_goal"<<"\t"<<"x_increasement"<<"\t"<<"y_increasement"<<"\t"
    <<"z_current"<<"\t"<<"concentration_1"<<"\t"<<"concentration_2"<<"\t"<<"quadrant"<<"\n"; 

    //开始寻源
    for(i=0;1;i++)
    {   

        ofstream fout(str);
        ros::spinOnce();

        listener.lookupTransform(odom_frame,base_frame, ros::Time(0), transform);
        x_current=transform.getOrigin().x();
        y_current=transform.getOrigin().y();
        z_current=transform.getRotation().getZ();
        quadrant=quadrantCaculator(z_current,sensor_data);
        
        fout<<i<<"\t"<<x_current<<"\t"<<y_current<<"\t"<<z_current<<
        "\t"<<sensor_data.concentration_1<<"\t"<<sensor_data.concentration_2<<"\t"<<quadrant<<"\n";

        ros::Duration(1.0).sleep();       
        fout.close();   
    }
    

    /*ROS_INFO("SEARCHING FININSHED");
    for(i=1;i<SEARCHING_TIME;i++)
        ROS_INFO("x=%f y=%f concentration 1=%f concentration 2=%f",goal[i].target_pose.pose.position.x,goal[i].target_pose.pose.position.y, current_concentration[i].concentration_1,current_concentration[i].concentration_2);
    ROS_INFO("SUCCESSED %d TIMES",successed_time);
    return 0;*/
}