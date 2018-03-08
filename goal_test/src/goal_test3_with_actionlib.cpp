#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <ros/time.h>
#include <tf/transform_listener.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <string.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
std_msgs::Float32 sensor_data;


//定义判断目标的是否在范围内的函数。
bool isInRange(double x,double y)
{
    if((x>-0.7)&&(x<3)&&(y>-1.7)&&(y<0.5))
        return true;
    else
        return false;
}

void sensor_subCallback(const std_msgs::Float32 &msg)
{
    sensor_data.data = msg.data;
    ROS_INFO("The sensor_data is %lf!",sensor_data.data); 
}

void assignGoal(move_base_msgs::MoveBaseGoal& next_goal,move_base_msgs::MoveBaseGoal& last_goal,double increasement_x,double increasement_y){
    next_goal.target_pose.header.frame_id = "odom";
    next_goal.target_pose.header.stamp = ros::Time::now();
    next_goal.target_pose.pose.position.x=last_goal.target_pose.pose.position.x+increasement_x; 
    next_goal.target_pose.pose.position.y=last_goal.target_pose.pose.position.y+increasement_y;
    next_goal.target_pose.pose.orientation.w = 1.0;
}

int main(int argc, char *argv[])
{
    //初始化节点
    ros::init(argc, argv, "simple_navigation_goals");
    
    //定义传感器订阅
    ros::NodeHandle n;
    ros::Subscriber sensor_sub;
    sensor_sub = n.subscribe("temperature", 1, sensor_subCallback);

    //定义actionlib的client
    MoveBaseClient ac("move_base", true);

    //等待actionlib的server响应
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    //定义tf相关常量
    tf::TransformListener listener;
    tf::StampedTransform transform;
    std::string odom_frame="/odom";
    std::string base_frame="/base_footprint";

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

    //定义目标点数组
    move_base_msgs::MoveBaseGoal goal[100];

    //定义温度值数组
    std_msgs::Float32 current_temperature[100];

    //起点赋值（必须全部赋值，否则发送目标将失败）
    goal[0].target_pose.header.frame_id = "odom";
    goal[0].target_pose.header.stamp = ros::Time::now();
    goal[0].target_pose.pose.position.x = x_start;
    goal[0].target_pose.pose.position.y = y_start;
    goal[0].target_pose.pose.orientation.w = 1.0;

    //发送坐标，查询目标完成状况
    ac.sendGoal(goal[0]);
    ac.waitForResult(ros::Duration(10.0));

    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward");
    else
        ROS_INFO("The base failed to move forward 1 meter for some reason");

    //休眠10s
    ros::Duration(10.0).sleep();

    //获取温度值
    ros::spinOnce();
    current_temperature[0].data=sensor_data.data;

    //定义循环次数
    int i;

    //定义increasement
    double increasement_x,increasement_y;



    for(i=1;i<10;i++)
    {       
        //计算increasement段
        increasement_x=0.1;
        increasement_y=0.1;
        
        //赋值 发送 等待
        assignGoal(goal[i],goal[i-1],increasement_x,increasement_y);

        ac.sendGoal(goal[i]);
        ac.waitForResult(ros::Duration(10.0));
        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("Hooray, the base moved 1 meter forward");
        else
            ROS_INFO("The base failed to move forward 1 meter for some reason");
        
        //原地等待10s
        ros::Duration(10.0).sleep();    
        //采集当前传感器所测温度值
        ros::spinOnce();
        current_temperature[i].data=sensor_data.data;
        ROS_INFO("%f",current_temperature[i].data);
        ROS_INFO("%d",i);
    }
    
    for(i=1;i<10;i++)
        ROS_INFO("%f",current_temperature[i].data);
    return 0;
}