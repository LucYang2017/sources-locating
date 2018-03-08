#include "ros/ros.h"
#include <std_msgs/Float32.h>
#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fake_sensor");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::Float32>("alcohol", 10);

  ros::Rate loop_rate(1);

  std_msgs::Float32 fake_alco_msg;
  fake_alco_msg.data=1;
  int i; 
  while (ros::ok())
  {
    i++;
    if(i%30>15)
      fake_alco_msg.data=500;
    else
      fake_alco_msg.data=10; 
    chatter_pub.publish(fake_alco_msg);
    ROS_INFO("Publishing fake sensor msgs  %lf!",fake_alco_msg.data);
    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
