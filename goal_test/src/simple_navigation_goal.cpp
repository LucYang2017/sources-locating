#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <ros/time.h>



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

  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "odom";
  goal.target_pose.header.stamp = ros::Time::now();
  int i=0;
  while(1){

      goal.target_pose.pose.position.x = -1.7;
      goal.target_pose.pose.position.y = -1.0;
      goal.target_pose.pose.orientation.w = 1.0;

      ROS_INFO("Sending goal");
      ac.sendGoal(goal);

      ac.waitForResult(ros::Duration(60.0));

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward %d",i);
      else
        ROS_INFO("The base failed to move forward 1 meter for some reason");
      i++;
      goal.target_pose.pose.position.x = -0.70;
      goal.target_pose.pose.position.y = -0.5;
      goal.target_pose.pose.orientation.w = 1.0;

            ROS_INFO("Sending goal");
      ac.sendGoal(goal);
      //ros::Duration(60.0).sleep();

      ac.waitForResult();

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward %d",i);
      else
        ROS_INFO("The base failed to move forward 1 meter for some reason");
      i++;
  }


  return 0;
}