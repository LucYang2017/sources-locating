#include "config.1.h"
#include <cmath>
#include <ctime>

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

void sendGoalAndWait(MoveBaseClient& actionlibClient,MoveBaseGoal& goal)
{
    actionlibClient.sendGoal(goal);
    actionlibClient.waitForResult(ros::Duration(10.0));
    if(actionlibClient.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        {ROS_INFO("Goal reached.");successed_time++;}
    else
        ROS_INFO("Failed to reach the goal.");
    //原地等待10s
    ros::Duration(15.0).sleep(); 
}

void assignGoal(move_base_msgs::MoveBaseGoal& goal,double x,double y){
    goal.target_pose.header.frame_id = "odom";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x=x; 
    goal.target_pose.pose.position.y=y;
    goal.target_pose.pose.orientation.w = 1.0;
}

void sensor_subCallback(const std_msgs::Float32 &msg)
{
    sensor_data.data = msg.data;
    ROS_INFO("The sensor_data is %lf.",sensor_data.data); 
}



int main(int argc, char *argv[])
{
    //初始化节点
    ros::init(argc, argv, "test_with_actionlib");    
    //定义传感器订阅
    ros::NodeHandle n;
    ros::Subscriber sensor_sub;
    sensor_sub = n.subscribe("alcohol", 1, sensor_subCallback);

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
    listener.lookupTransform(odom_frame,base_frame, ros::Time(0), transform);
    float x_start = transform.getOrigin().x();
    float y_start = transform.getOrigin().y();

    //起点赋值（必须全部赋值，否则发送目标将失败）
    goal[0].target_pose.header.frame_id = "odom";
    goal[0].target_pose.header.stamp = ros::Time::now();
    goal[0].target_pose.pose.position.x = x_start;
    goal[0].target_pose.pose.position.y = y_start;
    goal[0].target_pose.pose.orientation.w = 1.0;

    //发送坐标
    sendGoalAndWait(ac,goal[0]);//此处会立即返回已到达目标点，因为goal[0]即是起点

    //获取温度值
    ros::spinOnce();
    current_concentration[0].data=sensor_data.data;

    //定义循环次数
    int i;
    float x_current,y_current,x_increasement,y_increasement,x_goal,y_goal;

    for(i=1;i<SEARCHING_TIME;i++)
    {   
        ROS_INFO("SEARCHING No.%d",i); 

        listener.lookupTransform(odom_frame,base_frame, ros::Time(0), transform);
        x_current=transform.getOrigin().x();
        y_current=transform.getOrigin().y();
        x_increasement=0.2;
        y_increasement=0.2;   
        //计算increasement
        do{
            srand((unsigned)time(NULL));
            x_increasement=(rand()%20)/10.0-1;
            y_increasement=(rand()%20)/10.0-1;

            
                                  
            x_goal=x_current+x_increasement;
            y_goal=y_current+y_increasement;            
            }
        while(!isInRange(x_goal, y_goal));
        ROS_INFO("x_increasement=%f,y_increasement=%f",x_increasement,y_increasement);
        ROS_INFO("x_goal=%f,y_goal=%f",x_goal,y_goal);

        //赋值
        assignGoal(goal[i],x_goal,y_goal);
        sendGoalAndWait(ac,goal[i]);
  
        //采集当前传感器所测温度值
        ros::spinOnce();
        current_concentration[i].data=sensor_data.data;


        ROS_INFO("The concentration is %f.",current_concentration[i].data);        
    }

    ROS_INFO("SEARCHING FININSHED");
    for(i=1;i<SEARCHING_TIME;i++)
        ROS_INFO("x=%f y=%f concentration=%f",goal[i].target_pose.pose.position.x,goal[i].target_pose.pose.position.y, current_concentration[i].data);
    ROS_INFO("SUCCESSED %d TIMES",successed_time);
    return 0;
}