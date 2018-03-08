#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <ros/time.h>
#include <tf/transform_listener.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <string.h>

#define X_MIN -0.8
#define Y_MIN -2.8
#define X_MAX 3.8
#define Y_MAX 1.8
#define SEARCHING_TIME 20
/************************************一个酒精传感器版本****************************************/
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
typedef move_base_msgs::MoveBaseGoal MoveBaseGoal;

MoveBaseGoal goal[100];

std_msgs::Float32 current_concentration[100];
std_msgs::Float32 sensor_data;

int successed_time=0;

//定义increasement
double increasement_x,increasement_y;

bool isInRange(double x,double y);

void sendGoalAndWait(MoveBaseClient& actionlibClient,MoveBaseGoal& goal);

void assignGoal(move_base_msgs::MoveBaseGoal& next_goal,move_base_msgs::MoveBaseGoal& last_goal,double increasement_x,double increasement_y);

