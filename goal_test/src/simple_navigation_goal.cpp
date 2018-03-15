#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <ros/time.h>
#include "config.h"
#include <cmath>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  tf::TransformListener listener;
  tf::StampedTransform transform;
  std::string odom_frame="/map";
  std::string base_frame="/base_footprint";//两个frame的名称很重要

  try
  {
      listener.waitForTransform(odom_frame, base_frame, ros::Time(), ros::Duration(2.0) );
  }
  catch(tf::TransformException& ex)
  {
      ROS_ERROR("%s", ex.what());
  }



  //we'll send a goal to the robot to move 1 meter forward

int i=1;
double alpha;
  while(ros::ok()){

      i++;
      listener.lookupTransform(odom_frame , base_frame , ros::Time(0), transform);
      goal.target_pose.header.frame_id = "map";
      goal.target_pose.header.stamp = ros::Time::now();
      alpha=transform.getRotation().z();
      goal.target_pose.pose.position.x=transform.getOrigin().x()+cos(asin(alpha)*2); 
      goal.target_pose.pose.position.y=transform.getOrigin().y()+sin(asin(alpha)*2);
      goal.target_pose.pose.orientation.z = 1.0;

      ROS_INFO("x: %f  y: %f alpha: %f ",goal.target_pose.pose.position.x,goal.target_pose.pose.position.y,alpha);

      ROS_INFO("Sending goal");
      ac.sendGoal(goal);

      ac.waitForResult(ros::Duration(60.0));
      ros::Duration(3.0).sleep();

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward ");
      else
        ROS_INFO("The base failed to move forward 1 meter for some reason");
        }
  }



